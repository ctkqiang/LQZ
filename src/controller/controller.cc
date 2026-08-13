#include "controller.h"

#include <exception>
#include <iostream>
#include <string>

#include "evidence/evidence.h"
#include "util/args.h"
#include "util/json.h"
#include "version.h"

namespace {

// 操作注册表:交互菜单与 CLI 子命令共用同一份分发逻辑
struct OperationEntry {
    const char* menuId;        // 交互菜单编号
    const char* subcommand;    // CLI 子命令名
    const char* friendlyName;  // 执行中的提示语
    const char* successName;   // 成功后的提示语(空 = 不提示)
    void (Model::*run)();      // 无参执行体(connect 特判,run 为 nullptr)
    bool writesArtifacts;      // true = 操作后写 manifest/custody
};

const OperationEntry kOperations[] = {
    {"1", "dump-call-logs", "正在获取通话记录...", "通话记录已导出",
     &Model::DumpCallLogs, true},
    {"2", "dump-sms", "正在导出短信...", "短信已导出", &Model::DumpSMS, true},
    {"3", "dump-photos", "正在导出照片...", "照片已导出", &Model::DumpPhotos,
     true},
    {"4", "dump-videos", "正在导出视频...", "视频已导出", &Model::DumpVideos,
     true},
    {"5", "dump-apks", "正在导出应用...", "应用已导出", &Model::DumpApks, true},
    {"6", "dump-documents", "正在导出文档...", "文档已导出",
     &Model::DumpDocuments, true},
    {"7", "dump-sensitive", "正在提取敏感信息...", "信息提取完成",
     &Model::ExtractSensitiveInfo, true},
    {"8", "connect", "正在无线连接设备...", "无线连接完成", nullptr, false},
    {"9", "dump-network", "正在获取网络信息...", "网络信息已导出",
     &Model::GetWifiAddress, true},
    {"10", "dump-system", "正在导出系统信息...", "系统信息已导出",
     &Model::DumpSystemInfo, true},
    {"11", "dump-packages", "正在获取应用列表...", "应用列表已导出",
     &Model::ListPackages, true},
    {"12", "dump-whatsapp", "正在提取WhatsApp数据...", "WhatsApp数据已导出",
     &Model::ExtractWhatsApp, true},
    {"13", "dump-browser", "正在提取浏览器数据...", "浏览器数据已导出",
     &Model::ExtractBrowserData, true},
    {"14", "dump-notifications", "正在提取通知记录...", "通知记录已导出",
     &Model::ExtractNotifications, true},
    {"15", "extract-wechat", "正在提取微信数据...", "微信数据已导出",
     &Model::Extract微信, true},
};

const OperationEntry* FindByMenuId(const std::string& menuId) {
    for (const auto& entry : kOperations) {
        if (menuId == entry.menuId) return &entry;
    }
    return nullptr;
}

const OperationEntry* FindBySubcommand(const std::string& subcommand) {
    for (const auto& entry : kOperations) {
        if (subcommand == entry.subcommand) return &entry;
    }
    return nullptr;
}

// 交互模式的历史别名(照旧映射到编号)
const char* ResolveInteractiveAlias(const std::string& input) {
    if (input == "wifi") return "9";
    if (input == "sysinfo") return "10";
    if (input == "packages") return "11";
    if (input == "whatsapp") return "12";
    if (input == "browser") return "13";
    if (input == "notifications") return "14";
    if (input == "dumpsys") return "15";
    return input.c_str();
}

// usage 文本:退出码约定的唯一出处
const char* Usage() {
    return
        "用法: lqz <子命令> [选项]\n"
        "\n"
        "子命令:\n"
        "  dump-call-logs      导出通话记录          dump-photos       导出照片\n"
        "  dump-sms            导出短信              dump-videos       导出视频\n"
        "  dump-documents      导出文档              dump-apks         导出应用\n"
        "  dump-sensitive      提取敏感信息          dump-network      获取WiFi信息\n"
        "  dump-system         导出系统信息          dump-packages     应用列表\n"
        "  dump-whatsapp       提取WhatsApp数据      dump-browser      浏览器数据\n"
        "  dump-notifications  通知记录              extract-wechat    提取微信数据\n"
        "  connect <ip>        无线连接设备          verify            校验 custody 链\n"
        "\n"
        "通用选项:\n"
        "  -s, --serial <序列号>   目标设备序列号(adb -s)\n"
        "  -o, --output <目录>     输出目录(默认 Datas/<子命令>;verify 时必填)\n"
        "      --json              结果以单行 JSON 输出到 stdout\n"
        "      --deep              配合 verify:逐文件校验内容哈希\n"
        "  -h, --help              显示本帮助\n"
        "  -V, --version           显示版本号\n"
        "\n"
        "退出码:\n"
        "  0  成功(含 --help/--version)\n"
        "  1  提取失败\n"
        "  2  参数错误\n"
        "  3  设备离线\n"
        "\n"
        "证据完整性:\n"
        "  每次提取后会在输出目录生成 manifest.json(文件清单 + SHA-256)\n"
        "  与 custody.log(防篡改哈希链)，可用 `lqz verify -o <目录>` 校验。\n"
        "\n"
        "无参数启动进入交互式界面。\n";
}

}  // namespace

int Controller::Run() {
    // 让 View 小助手先把屏幕擦得亮晶晶的~
    consoleUserInterface.ClearScreen();
    // 展示一个美美的欢迎标题页，就像拉开魔法舞台的帷幕~
    consoleUserInterface.DisplayHeader();

    try {
        // 让 Model 小医生去检查 ADB 工具有没有安装好哟~
        forensicDataModel.CheckAdb();
    } catch ([[maybe_unused]] const std::exception& caughtAdbCheckException) {
        // 如果检查失败，View 小助手会用红色字体伤心地报告错误
        consoleUserInterface.ShowError("ADB安装检查失败");
        return 1;
    }

    // 让 Model 小门卫检查一下有没有连接安卓设备呢~
    bool isDeviceConnected = forensicDataModel.CheckDevice();
    // View 小助手会用可爱的图标显示设备连接状态和型号哟
    consoleUserInterface.ShowDeviceStatus(isDeviceConnected,
                                          forensicDataModel.GetDeviceModel());
    // 展示一个小帮助菜单，就像递给用户一本魔法说明书~
    consoleUserInterface.DisplayHelp();

    // 如果没有检测到设备，View 小助手会用红色字体提醒我们哟
    if (!isDeviceConnected) {
        consoleUserInterface.ShowError("未检测到手机设备");
        return 0;
    }

    // 准备一个小本子，用来记录用户输入的命令哒
    std::string userInputCommand;
    // 进入一个魔法循环，直到用户说退出哟
    while (true) {
        // View 小助手会用粉色提示符提示用户输入命令哒
        consoleUserInterface.GetInput("\n  >> ", userInputCommand);

        try {
            // 如果输入的是 exit 或 0，就像小仙子要回家啦~
            if (userInputCommand == "exit" || userInputCommand == "0") {
                consoleUserInterface.ShowMessage(
                    "正在退出程序...", consoleUserInterface.YELLOW);
                break;
            }
            // 如果输入 clear，View 小助手会把屏幕擦得像新的一样哟
            if (userInputCommand == "clear") {
                consoleUserInterface.ClearScreen();
                continue;
            }
            // 如果输入 about，View 小助手会重新展示欢迎标题页哒
            if (userInputCommand == "about") {
                consoleUserInterface.DisplayHeader();
                continue;
            }
            // 如果输入 help 或 ?，View 小助手会再次展示帮助菜单哟
            if (userInputCommand == "help" || userInputCommand == "?") {
                consoleUserInterface.DisplayHelp();
                continue;
            }

            // 通过操作注册表分发(交互与 CLI 共用同一份逻辑)
            const char* menuId = ResolveInteractiveAlias(userInputCommand);
            if (runOperation(menuId, false) == -1) {
                consoleUserInterface.ShowError(
                    "未知命令，输入 'help' 或 '?' 查看可用命令");
            }

            // 检查一下设备连接状态，如果断开就结束魔法哟
            if (!forensicDataModel.CheckDevice()) {
                consoleUserInterface.ShowError("设备连接已断开");
                break;
            }
        } catch (const std::exception& caughtException) {
            // 如果执行命令时出错，View 小助手会用红色字体报告错误信息哒
            consoleUserInterface.ShowError(
                std::string("执行命令时出错: ") + caughtException.what());
        }
    }
    return 0;
}

// 交互与 CLI 共享的执行体:menuId 找不到返回 -1,执行失败返回 1,成功 0
int Controller::runOperation(const std::string& menuId, bool jsonMode) {
    const OperationEntry* entry = FindByMenuId(menuId);
    if (entry == nullptr) return -1;

    lastCliError_.clear();
    if (!jsonMode) {
        consoleUserInterface.ShowMessage(entry->friendlyName,
                                         consoleUserInterface.BLUE);
    }
    try {
        if (entry->run != nullptr) {
            (forensicDataModel.*(entry->run))();
        } else {
            // connect 特判:CLI 有 IP 参数,交互模式读输入
            if (!pendingConnectIp_.empty()) {
                forensicDataModel.ConnectWirelessly(pendingConnectIp_);
                pendingConnectIp_.clear();
            } else {
                forensicDataModel.ConnectWirelessly();
            }
        }
    } catch (const std::exception& caughtException) {
        lastCliError_ =
            std::string("执行命令时出错: ") + caughtException.what();
        if (!jsonMode) consoleUserInterface.ShowError(lastCliError_);
        return 1;
    }

    // 证据记录:取证合规的核心一步
    if (entry->writesArtifacts) {
        std::string evidenceError;
        std::string manifestSha256;
        std::size_t artifactCount = 0;
        if (!forensicDataModel.RecordEvidence(entry->subcommand, evidenceError,
                                              &manifestSha256,
                                              &artifactCount)) {
            lastCliError_ = "证据记录失败: " + evidenceError;
            if (!jsonMode) consoleUserInterface.ShowError(lastCliError_);
            return 1;
        }
        lastManifestSha256_ = manifestSha256;
        lastArtifactCount_ = artifactCount;
    } else if (!forensicDataModel.LastCommandSucceeded()) {
        lastCliError_ = "操作失败";
        if (!jsonMode) consoleUserInterface.ShowError(lastCliError_);
        return 1;
    }

    if (!jsonMode && entry->successName[0] != '\0') {
        consoleUserInterface.ShowSuccess(entry->successName);
    }
    return 0;
}

// --json 结果输出(stdout 唯一内容)
void Controller::emitJsonResult(const std::string& command, int exitCode,
                                bool withArtifacts) {
    util::JsonWriter w(false);
    w.BeginObject();
    if (exitCode == 0) {
        w.Key("status");
        w.String("ok");
        w.Key("command");
        w.String(command);
        w.Key("exit_code");
        w.Number(static_cast<std::int64_t>(0));
        if (withArtifacts) {
            w.Key("output_dir");
            w.String(lastOutputRoot_);
            w.Key("manifest");
            w.String(lastOutputRoot_ + "/manifest.json");
            w.Key("manifest_sha256");
            w.String(lastManifestSha256_);
            w.Key("artifacts");
            w.Number(static_cast<std::int64_t>(lastArtifactCount_));
        }
    } else {
        w.Key("status");
        w.String("error");
        w.Key("command");
        w.String(command);
        w.Key("exit_code");
        w.Number(static_cast<std::int64_t>(exitCode));
        w.Key("message");
        w.String(lastCliError_);
    }
    w.EndObject();
    std::cout << w.Take() << "\n";
}

int Controller::RunCli(int argc, char* argv[]) {
    Controller controller;
    return controller.runCliImpl(argc, argv);
}

int Controller::runCliImpl(int argc, char* argv[]) {
    // 1) 解析参数
    util::ParsedArgs args;
    std::string parseError;
    if (util::ParseArgs(argc, argv, args, parseError) != 0) {
        std::cerr << "参数错误: " << parseError << "\n\n" << Usage();
        return 2;
    }
    if (args.helpFlag || args.command == "help") {
        std::cout << Usage();
        return 0;
    }
    if (args.versionFlag || args.command == "version") {
        std::cout << "lqz " << LQZ_VERSION_STRING << "\n";
        return 0;
    }
    if (args.command.empty()) {
        std::cerr << "缺少子命令\n\n" << Usage();
        return 2;
    }

    // 2) verify:校验 custody 链(无需设备)
    if (args.command == "verify") {
        if (args.outputDir.empty()) {
            std::cerr << "verify 需要 -o <输出目录>\n";
            return 2;
        }
        const auto report = evidence::VerifyChain(args.outputDir, args.deepFlag);
        if (args.jsonFlag) {
            util::JsonWriter w(false);
            w.BeginObject();
            w.Key("status");
            w.String(report.ok ? "ok" : "error");
            w.Key("command");
            w.String("verify");
            w.Key("valid");
            w.Bool(report.ok);
            w.Key("entries");
            w.Number(static_cast<std::int64_t>(report.entries));
            w.Key("bad");
            w.Number(static_cast<std::int64_t>(report.bad));
            w.Key("issues");
            w.BeginArray();
            for (const auto& issue : report.issues) w.String(issue);
            w.EndArray();
            w.EndObject();
            std::cout << w.Take() << "\n";
        } else {
            if (report.ok) {
                std::cout << "[ ok ] custody 链有效，共 " << report.entries
                          << " 条记录\n";
            } else {
                for (const auto& issue : report.issues) {
                    std::cout << "[err] " << issue << "\n";
                }
            }
        }
        return report.ok ? 0 : 1;
    }

    // 3) connect:无需设备已在线(它本身就是用来建立连接的)
    if (args.command == "connect") {
        if (args.positional.empty()) {
            std::cerr << "connect 需要设备 IP，如: lqz connect 192.168.1.10\n";
            return 2;
        }
        pendingConnectIp_ = args.positional[0];
        consoleUserInterface.SetCliMode(true);
        forensicDataModel.SetQuiet(true);
        const int rc = runOperation("8", args.jsonFlag);
        if (args.jsonFlag) emitJsonResult("connect", rc == 0 ? 0 : 1, false);
        return rc == 0 ? 0 : 1;
    }

    // 4) 其余提取子命令
    const OperationEntry* entry = FindBySubcommand(args.command);
    if (entry == nullptr) {
        std::cerr << "未知子命令: " << args.command << "\n\n" << Usage();
        return 2;
    }

    if (!args.serial.empty()) forensicDataModel.SetDeviceSerial(args.serial);
    lastOutputRoot_ = args.outputDir.empty()
                          ? ("Datas/" + std::string(entry->subcommand))
                          : args.outputDir;
    forensicDataModel.SetOutputRoot(lastOutputRoot_);
    forensicDataModel.SetQuiet(true);
    consoleUserInterface.SetCliMode(true);

    // 设备在线检查(失败退出码 3)
    if (!forensicDataModel.IsDeviceOnline()) {
        lastCliError_ = "设备离线或未授权";
        if (args.jsonFlag) emitJsonResult(args.command, 3, true);
        else std::cerr << "  [err] " << lastCliError_ << "\n";
        return 3;
    }

    const int rc = runOperation(entry->menuId, args.jsonFlag);
    if (args.jsonFlag) emitJsonResult(args.command, rc == 0 ? 0 : 1, true);
    return rc == 0 ? 0 : 1;
}
