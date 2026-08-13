#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "evidence/evidence.h"
#include "test_framework.h"
#include "util/sha256.h"
#include "version.h"

namespace {

// 建一个含两个文件的临时输出目录
std::filesystem::path MakeSampleDir() {
    static int counter = 0;
    const auto base = std::filesystem::temp_directory_path() /
                      ("lqz_test_" + std::to_string(++counter));
    std::filesystem::remove_all(base);
    std::filesystem::create_directories(base / "sub");
    {
        std::ofstream f(base / "a.txt");
        f << "hello evidence\n";
    }
    {
        std::ofstream f(base / "sub" / "b.bin");
        f << "binary payload";
    }
    return base;
}

evidence::ManifestMeta MakeMeta() {
    evidence::ManifestMeta meta;
    meta.command = "dump-test";
    meta.toolVersion = LQZ_VERSION_STRING;
    meta.serial = "TEST123";
    meta.deviceModel = "device:testphone";
    return meta;
}

// 走完整流程:manifest + custody,返回 manifest 摘要
std::string BuildChain(const std::filesystem::path& dir,
                       const evidence::ManifestMeta& meta) {
    std::vector<evidence::ArtifactInfo> artifacts;
    std::string error;
    if (!evidence::WriteManifest(dir, meta, artifacts, error)) return "";
    std::string hashError;
    const std::string manifestSha =
        util::Sha256FileHex((dir / "manifest.json").string(), hashError);
    if (!evidence::AppendCustody(dir, meta, artifacts, manifestSha, error)) {
        return "";
    }
    return manifestSha;
}

// 读取 custody.log 全文
std::string ReadCustody(const std::filesystem::path& dir) {
    std::ifstream f(dir / "custody.log");
    std::string content((std::istreambuf_iterator<char>(f)),
                        std::istreambuf_iterator<char>());
    return content;
}

// 覆盖写回 custody.log
void WriteCustody(const std::filesystem::path& dir, const std::string& content) {
    std::ofstream f(dir / "custody.log", std::ios::trunc);
    f << content;
}

}  // namespace

TEST_GROUP(manifest_lists_files_with_hashes) {
    const auto dir = MakeSampleDir();
    const auto meta = MakeMeta();
    std::vector<evidence::ArtifactInfo> artifacts;
    std::string error;
    CHECK(evidence::WriteManifest(dir, meta, artifacts, error));
    CHECK_EQ(artifacts.size(), 2u);
    // 路径排序且统一 '/' 分隔
    CHECK_EQ(artifacts[0].path, "a.txt");
    CHECK_EQ(artifacts[1].path, "sub/b.bin");
    CHECK_EQ(artifacts[0].sha256, util::Sha256Hex("hello evidence\n"));
    CHECK(artifacts[0].sizeBytes > 0);
    CHECK(artifacts[0].mtimeUtc.size() >= 20);  // ISO-8601 UTC 时间戳
    std::filesystem::remove_all(dir);
}

TEST_GROUP(custody_build_and_verify) {
    const auto dir = MakeSampleDir();
    const auto meta = MakeMeta();
    CHECK(!BuildChain(dir, meta).empty());

    const auto report = evidence::VerifyChain(dir, false);
    CHECK(report.ok);
    CHECK_EQ(report.entries, 2u);  // genesis + extraction_complete
    CHECK(report.issues.empty());

    const auto deep = evidence::VerifyChain(dir, true);
    CHECK(deep.ok);

    std::filesystem::remove_all(dir);
}

TEST_GROUP(custody_detect_tampered_record) {
    const auto dir = MakeSampleDir();
    const auto meta = MakeMeta();
    BuildChain(dir, meta);

    // 篡改第 2 行(extraction 记录)行尾 hash 的一个字符。
    // 行尾字节布局: ...<hash 64hex>"}\n,hash 尾字符在 size-4
    std::string content = ReadCustody(dir);
    const auto secondLineStart = content.find('\n');
    if (secondLineStart != std::string::npos) {
        const std::size_t pos = content.size() - 4;
        content[pos] = (content[pos] == '0') ? '1' : '0';
    }
    WriteCustody(dir, content);

    const auto report = evidence::VerifyChain(dir, false);
    CHECK(!report.ok);
    CHECK(report.bad > 0);

    std::filesystem::remove_all(dir);
}

TEST_GROUP(custody_detect_reordered_records) {
    const auto dir = MakeSampleDir();
    const auto meta = MakeMeta();
    BuildChain(dir, meta);

    // 交换前两行:第 2 条的 prev_hash 不再指向上一条
    const std::string content = ReadCustody(dir);
    const auto firstEnd = content.find('\n');
    const auto secondEnd =
        (firstEnd == std::string::npos) ? std::string::npos
                                        : content.find('\n', firstEnd + 1);
    if (firstEnd != std::string::npos && secondEnd != std::string::npos) {
        const std::string first = content.substr(0, firstEnd);
        const std::string second = content.substr(firstEnd + 1, secondEnd - firstEnd - 1);
        const std::string rest = content.substr(secondEnd + 1);
        WriteCustody(dir, second + "\n" + first + "\n" + rest);
    }

    const auto report = evidence::VerifyChain(dir, false);
    CHECK(!report.ok);

    std::filesystem::remove_all(dir);
}

TEST_GROUP(custody_detect_manifest_change) {
    const auto dir = MakeSampleDir();
    const auto meta = MakeMeta();
    BuildChain(dir, meta);

    // 追加篡改内容:manifest 摘要与 custody 末条记录对不上
    {
        std::ofstream f(dir / "manifest.json", std::ios::app);
        f << "tampered";
    }
    const auto report = evidence::VerifyChain(dir, false);
    CHECK(!report.ok);

    std::filesystem::remove_all(dir);
}

TEST_GROUP(custody_detect_artifact_change_deep) {
    const auto dir = MakeSampleDir();
    const auto meta = MakeMeta();
    BuildChain(dir, meta);

    // 改提取物文件内容:浅校验(链 + manifest)仍通过,--deep 才能检出
    {
        std::ofstream f(dir / "a.txt", std::ios::app);
        f << "tampered!";
    }
    const auto shallow = evidence::VerifyChain(dir, false);
    CHECK(shallow.ok);
    const auto deep = evidence::VerifyChain(dir, true);
    CHECK(!deep.ok);

    std::filesystem::remove_all(dir);
}

TEST_GROUP(custody_missing_log) {
    const auto dir = MakeSampleDir();
    const auto report = evidence::VerifyChain(dir, false);
    CHECK(!report.ok);
    CHECK(!report.issues.empty());
    std::filesystem::remove_all(dir);
}
