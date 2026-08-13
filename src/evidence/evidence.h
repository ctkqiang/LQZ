#ifndef EVIDENCE_EVIDENCE_H
#define EVIDENCE_EVIDENCE_H

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

// 证据完整性模块:manifest(文件清单 + SHA-256)+ 防篡改链式 custody log + verify。
//
// 约定(第三方核验请以此为准):
// 1. JSON 键顺序固定:写入顺序即键序,verify 依赖字节级一致性。
// 2. custody 每条记录的 "hash" 字段恒为行尾字段;hash = sha256_hex(prev_hash + "|" + canonical),
//    其中 canonical 为去掉 hash 字段后的紧凑 JSON(与写入行字节一致)。
// 3. 首条记录为创世记录(chain_genesis,prev_hash 为 64 个 0)。
// 4. 截断链尾:若保留 manifest.json 且末条记录含 manifest_sha256,可通过对拍检出;
//    链尾完整性锚点见每次提取完成时输出的最新 hash。
namespace evidence {

struct ArtifactInfo {
    std::string path;        // 相对路径,统一用 '/' 分隔
    std::uintmax_t sizeBytes = 0;
    std::string sha256;      // 64 位小写 hex
    std::string mtimeUtc;    // ISO-8601 UTC,如 2026-08-13T12:00:00Z
};

struct ManifestMeta {
    std::string command;      // 如 "dump-sms"
    std::string toolVersion;  // LQZ_VERSION_STRING
    std::string serial;       // adb -s 的序列号,可为空
    std::string deviceModel;  // CheckDevice 抓到的型号,可为空
};

// 递归扫描 outDir 生成 manifest.json(跳过 manifest.json 自身与 custody.log)。
// 成功时 outArtifacts 为按路径排序的文件清单;失败返回 false 并写明 errorOut。
bool WriteManifest(const std::filesystem::path& outDir, const ManifestMeta& meta,
                   std::vector<ArtifactInfo>& outArtifacts,
                   std::string& errorOut);

// 追加一条 custody 记录(extraction_complete)。
// 日志不存在时先写入创世记录(chain_genesis);manifestSha256 为 manifest.json 的摘要。
bool AppendCustody(const std::filesystem::path& outDir, const ManifestMeta& meta,
                   const std::vector<ArtifactInfo>& artifacts,
                   const std::string& manifestSha256, std::string& errorOut);

struct VerifyReport {
    bool ok = false;
    std::size_t entries = 0;  // 校验的 custody 记录数
    std::size_t bad = 0;      // 问题数
    std::vector<std::string> issues;
};

// 校验 custody 哈希链;deep 时额外逐文件对拍磁盘内容与 manifest 记录。
VerifyReport VerifyChain(const std::filesystem::path& outDir, bool deep);

// 时间与操作者工具
std::string Iso8601UtcNow();
std::string Iso8601FromFileTime(const std::filesystem::file_time_type& ft);
std::string GetOperatorName();  // $USER/$USERNAME,均无则 "unknown"

}  // namespace evidence

#endif
