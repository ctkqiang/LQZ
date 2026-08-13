#include "evidence/evidence.h"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>

#include "util/json.h"
#include "util/sha256.h"
#include "version.h"

namespace evidence {
namespace {

constexpr std::size_t kSha256HexLength = 64;
constexpr std::size_t kGenesisPrevHashLength = 64;

// 固定键顺序序列化 custody 记录(不含 hash 字段,由调用方补行尾)。
// 键序:index, prev_hash, timestamp_utc, event_type, operator, tool_version,
//       command, serial, device_model, manifest_sha256, artifacts
std::string SerializeCustodyRecord(std::int64_t index,
                                   const std::string& prevHash,
                                   const std::string& timestampUtc,
                                   const std::string& eventType,
                                   const std::string& operatorName,
                                   const std::string& toolVersion,
                                   const std::string& command,
                                   const std::string& serial,
                                   const std::string& deviceModel,
                                   const std::string& manifestSha256,
                                   const std::vector<ArtifactInfo>& artifacts) {
    util::JsonWriter w(false);  // 紧凑:单行
    w.BeginObject();
    w.Key("index");
    w.Number(index);
    w.Key("prev_hash");
    w.String(prevHash);
    w.Key("timestamp_utc");
    w.String(timestampUtc);
    w.Key("event_type");
    w.String(eventType);
    w.Key("operator");
    w.String(operatorName);
    w.Key("tool_version");
    w.String(toolVersion);
    w.Key("command");
    w.String(command);
    w.Key("serial");
    w.String(serial);
    w.Key("device_model");
    w.String(deviceModel);
    w.Key("manifest_sha256");
    w.String(manifestSha256);
    w.Key("artifacts");
    w.BeginArray();
    for (const auto& artifact : artifacts) {
        w.BeginObject();
        w.Key("path");
        w.String(artifact.path);
        w.Key("sha256");
        w.String(artifact.sha256);
        w.EndObject();
    }
    w.EndArray();
    w.EndObject();
    return w.Take();
}

// 哈希链:hash_i = sha256_hex(prev_hash_i + "|" + canonical_i)
std::string ComputeRecordHash(const std::string& prevHash,
                              const std::string& canonical) {
    return util::Sha256Hex(prevHash + "|" + canonical);
}

// 给 canonical(以 '}' 结尾的紧凑 JSON)补上 hash 字段形成完整记录行
std::string AppendHashField(const std::string& canonical,
                            const std::string& hash) {
    std::string line = canonical;
    line.pop_back();  // 去尾 '}'
    line += ",\"hash\":\"" + hash + "\"}";
    return line;
}

bool ReadFileContent(const std::filesystem::path& path, std::string& out) {
    std::ifstream file(path, std::ios::binary);
    if (!file) return false;
    std::ostringstream buffer;
    buffer << file.rdbuf();
    out = buffer.str();
    return true;
}

// 读取 custody.log 最后一条有效记录的 index 与 hash;
// 日志不存在或为空时 lastIndex = -1(表示需要写创世记录)
bool ReadLastCustody(const std::filesystem::path& custodyPath,
                     std::int64_t& lastIndex, std::string& lastHash,
                     std::string& errorOut) {
    lastIndex = -1;
    lastHash = std::string(kGenesisPrevHashLength, '0');
    std::ifstream file(custodyPath);
    if (!file) {
        if (std::filesystem::exists(custodyPath)) {
            errorOut = "无法读取 custody.log";
            return false;
        }
        return true;  // 不存在 → 从创世开始
    }
    std::string lastLine;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) lastLine = line;
    }
    if (lastLine.empty()) return true;  // 空文件视同不存在

    const std::string marker = ",\"hash\":\"";
    const auto pos = lastLine.rfind(marker);
    if (pos == std::string::npos ||
        lastLine.size() < pos + marker.size() + kSha256HexLength + 1) {
        errorOut = "custody.log 最后一条记录格式损坏";
        return false;
    }
    lastHash = lastLine.substr(pos + marker.size(), kSha256HexLength);

    const std::string indexPrefix = "{\"index\":";
    if (lastLine.compare(0, indexPrefix.size(), indexPrefix) != 0) {
        errorOut = "custody.log 最后一条记录缺少 index 字段";
        return false;
    }
    lastIndex =
        std::strtoll(lastLine.c_str() + indexPrefix.size(), nullptr, 10);
    return true;
}

std::string FormatSystemTime(const std::chrono::system_clock::time_point& tp) {
    const std::time_t tt = std::chrono::system_clock::to_time_t(tp);
    std::tm tmv{};
#if defined(_WIN32) && !defined(__MINGW32__)
    gmtime_s(&tmv, &tt);
#else
    gmtime_r(&tt, &tmv);
#endif
    char buffer[32];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%SZ", &tmv);
    return buffer;
}

}  // namespace

bool WriteManifest(const std::filesystem::path& outDir,
                   const ManifestMeta& meta,
                   std::vector<ArtifactInfo>& outArtifacts,
                   std::string& errorOut) {
    if (!std::filesystem::exists(outDir)) {
        errorOut = "输出目录不存在: " + outDir.string();
        return false;
    }

    std::vector<ArtifactInfo> artifacts;
    for (std::filesystem::recursive_directory_iterator it(
             outDir, std::filesystem::directory_options::skip_permission_denied),
         end;
         it != end; ++it) {
        if (!it->is_regular_file()) continue;
        const auto relativePath = std::filesystem::relative(it->path(), outDir);
        if (relativePath.filename() == "manifest.json" ||
            relativePath.filename() == "custody.log") {
            continue;  // 跳过证据文件自身
        }
        std::string hashError;
        const std::string hash = util::Sha256FileHex(it->path().string(), hashError);
        if (hash.empty()) {
            errorOut = hashError;
            return false;
        }
        ArtifactInfo info;
        info.path = relativePath.generic_string();  // generic_string 统一 '/'
        info.sizeBytes = it->file_size();
        info.sha256 = hash;
        info.mtimeUtc = Iso8601FromFileTime(it->last_write_time());
        artifacts.push_back(std::move(info));
    }
    std::sort(artifacts.begin(), artifacts.end(),
              [](const ArtifactInfo& a, const ArtifactInfo& b) {
                  return a.path < b.path;
              });

    util::JsonWriter w(true);
    w.BeginObject();
    w.Key("manifest_version");
    w.Number(static_cast<std::int64_t>(1));
    w.Key("tool");
    w.String("lqz");
    w.Key("tool_version");
    w.String(meta.toolVersion);
    w.Key("command");
    w.String(meta.command);
    w.Key("serial");
    w.String(meta.serial);
    w.Key("device_model");
    w.String(meta.deviceModel);
    w.Key("created_at_utc");
    w.String(Iso8601UtcNow());
    w.Key("files");
    w.BeginArray();
    for (const auto& artifact : artifacts) {
        w.BeginObject();
        w.Key("path");
        w.String(artifact.path);
        w.Key("size_bytes");
        w.Number(static_cast<std::uint64_t>(artifact.sizeBytes));
        w.Key("sha256");
        w.String(artifact.sha256);
        w.Key("mtime_utc");
        w.String(artifact.mtimeUtc);
        w.EndObject();
    }
    w.EndArray();
    w.EndObject();

    std::ofstream file(outDir / "manifest.json", std::ios::binary);
    if (!file) {
        errorOut = "无法写入 manifest.json";
        return false;
    }
    file << w.Take() << '\n';
    if (!file) {
        errorOut = "写入 manifest.json 失败";
        return false;
    }
    outArtifacts = artifacts;
    return true;
}

bool AppendCustody(const std::filesystem::path& outDir,
                   const ManifestMeta& meta,
                   const std::vector<ArtifactInfo>& artifacts,
                   const std::string& manifestSha256,
                   std::string& errorOut) {
    const auto custodyPath = outDir / "custody.log";
    const std::string operatorName = GetOperatorName();
    const std::string timestampUtc = Iso8601UtcNow();

    std::int64_t lastIndex = -1;
    std::string lastHash;
    if (!ReadLastCustody(custodyPath, lastIndex, lastHash, errorOut)) {
        return false;
    }

    std::ofstream file(custodyPath, std::ios::app);
    if (!file) {
        errorOut = "无法打开 custody.log";
        return false;
    }

    if (lastIndex < 0) {
        // 创世记录:index 0,prev_hash 全零,字段留空
        const std::string canonical = SerializeCustodyRecord(
            0, std::string(kGenesisPrevHashLength, '0'), timestampUtc,
            "chain_genesis", operatorName, meta.toolVersion, "", "", "", "",
            {});
        const std::string hash =
            ComputeRecordHash(std::string(kGenesisPrevHashLength, '0'), canonical);
        file << AppendHashField(canonical, hash) << '\n';
        lastHash = hash;
        lastIndex = 0;
    }

    // 本次提取记录
    const std::string canonical = SerializeCustodyRecord(
        lastIndex + 1, lastHash, timestampUtc, "extraction_complete",
        operatorName, meta.toolVersion, meta.command, meta.serial,
        meta.deviceModel, manifestSha256, artifacts);
    const std::string hash = ComputeRecordHash(lastHash, canonical);
    file << AppendHashField(canonical, hash) << '\n';
    if (!file) {
        errorOut = "写入 custody.log 失败";
        return false;
    }
    return true;
}

VerifyReport VerifyChain(const std::filesystem::path& outDir, bool deep) {
    VerifyReport report;
    const auto custodyPath = outDir / "custody.log";
    std::ifstream file(custodyPath);
    if (!file) {
        report.issues.push_back("custody.log 不存在或无法读取");
        ++report.bad;
        return report;
    }

    std::string prevHash(kGenesisPrevHashLength, '0');
    std::string line;
    std::size_t lineNo = 0;
    std::string lastManifestSha256;
    bool haveLastManifestSha256 = false;
    bool lastIsExtraction = false;

    while (std::getline(file, line)) {
        ++lineNo;
        if (line.empty()) continue;
        ++report.entries;

        // 1) 提取行尾 hash 字段并还原 canonical
        const std::string marker = ",\"hash\":\"";
        const auto pos = line.rfind(marker);
        if (pos == std::string::npos ||
            line.size() < pos + marker.size() + kSha256HexLength + 1) {
            report.issues.push_back("第 " + std::to_string(lineNo) +
                                    " 行格式损坏(缺少 hash 字段)");
            ++report.bad;
            continue;
        }
        const std::string recordedHash =
            line.substr(pos + marker.size(), kSha256HexLength);
        const std::string canonical = line.substr(0, pos) + "}";

        // 2) 重算哈希:prev_hash、字段内容、记录顺序的任何变动都会导致不匹配
        if (ComputeRecordHash(prevHash, canonical) != recordedHash) {
            report.issues.push_back("第 " + std::to_string(lineNo) +
                                    " 行哈希不匹配(记录被篡改或顺序错乱)");
            ++report.bad;
        }
        prevHash = recordedHash;

        // 3) 提取 event_type 与 manifest_sha256(子串定位,hex 无转义风险)
        const std::string eventMarker = "\"event_type\":\"";
        const auto eventPos = line.find(eventMarker);
        if (eventPos != std::string::npos) {
            const auto eventEnd = line.find('"', eventPos + eventMarker.size());
            if (eventEnd != std::string::npos) {
                lastIsExtraction =
                    line.substr(eventPos + eventMarker.size(),
                                eventEnd - eventPos - eventMarker.size()) ==
                    "extraction_complete";
            }
        }
        const std::string manifestMarker = "\"manifest_sha256\":\"";
        const auto manifestPos = line.find(manifestMarker);
        if (manifestPos != std::string::npos &&
            line.size() >= manifestPos + manifestMarker.size() + kSha256HexLength) {
            lastManifestSha256 =
                line.substr(manifestPos + manifestMarker.size(), kSha256HexLength);
            haveLastManifestSha256 = !lastManifestSha256.empty();
        }
    }

    // 4) 与磁盘 manifest.json 对拍(检出 manifest 被改/删)
    std::string manifestText;
    if (haveLastManifestSha256) {
        std::string hashError;
        const std::string currentSha =
            util::Sha256FileHex((outDir / "manifest.json").string(), hashError);
        if (currentSha.empty()) {
            report.issues.push_back("manifest.json 不存在或无法读取");
            ++report.bad;
        } else if (currentSha != lastManifestSha256) {
            report.issues.push_back(
                "manifest.json 与 custody 链末条记录不一致(manifest 被篡改?)");
            ++report.bad;
        }
    }

    // 5) deep:逐文件对拍磁盘内容与 manifest 文本中的记录
    if (deep && lastIsExtraction) {
        if (ReadFileContent(outDir / "manifest.json", manifestText)) {
            for (std::filesystem::recursive_directory_iterator it(
                     outDir, std::filesystem::directory_options::skip_permission_denied),
                 end;
                 it != end; ++it) {
                if (!it->is_regular_file()) continue;
                const auto relativePath =
                    std::filesystem::relative(it->path(), outDir);
                if (relativePath.filename() == "manifest.json" ||
                    relativePath.filename() == "custody.log") {
                    continue;
                }
                const std::string relativePathString = relativePath.generic_string();
                // 与写入时的转义规则一致地定位记录(pretty 格式: "path": "<escaped>")
                const std::string needle =
                    "\"path\": \"" + util::JsonEscape(relativePathString) + "\"";
                const auto pathPos = manifestText.find(needle);
                if (pathPos == std::string::npos) {
                    report.issues.push_back("manifest 缺少文件记录: " +
                                            relativePathString);
                    ++report.bad;
                    continue;
                }
                const std::string shaMarker = "\"sha256\": \"";
                const auto shaPos = manifestText.find(shaMarker, pathPos);
                if (shaPos == std::string::npos ||
                    manifestText.size() < shaPos + shaMarker.size() + kSha256HexLength) {
                    report.issues.push_back("manifest 记录格式损坏: " +
                                            relativePathString);
                    ++report.bad;
                    continue;
                }
                const std::string recordedSha =
                    manifestText.substr(shaPos + shaMarker.size(), kSha256HexLength);
                std::string hashError;
                const std::string actualSha =
                    util::Sha256FileHex(it->path().string(), hashError);
                if (actualSha != recordedSha) {
                    report.issues.push_back("文件内容与 manifest 不一致: " +
                                            relativePathString);
                    ++report.bad;
                }
            }
        } else {
            report.issues.push_back("--deep 校验需要 manifest.json");
            ++report.bad;
        }
    }

    report.ok = (report.bad == 0) && (report.entries > 0);
    return report;
}

std::string Iso8601UtcNow() {
    return FormatSystemTime(std::chrono::system_clock::now());
}

std::string Iso8601FromFileTime(const std::filesystem::file_time_type& ft) {
    // 可移植转换:Windows 上 file_time_type 是 FILETIME(1601 纪元),
    // 不能直接转 system_clock,用"差值 + 当前时刻"对齐(跨平台标准写法)
    const auto systemTime =
        std::chrono::time_point_cast<std::chrono::system_clock::duration>(
            ft - std::filesystem::file_time_type::clock::now() +
            std::chrono::system_clock::now());
    return FormatSystemTime(systemTime);
}

std::string GetOperatorName() {
    const char* user = std::getenv("USER");
    if (user == nullptr || *user == '\0') user = std::getenv("USERNAME");
    return (user != nullptr && *user != '\0') ? user : "unknown";
}

}  // namespace evidence
