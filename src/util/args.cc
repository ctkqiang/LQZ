#include "util/args.h"

namespace util {

int ParseArgs(int argc, char* argv[], ParsedArgs& out, std::string& error) {
    bool positionalOnly = false;  // "--" 之后只收位置参数
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];

        if (!positionalOnly && arg == "--") {
            positionalOnly = true;
            continue;
        }
        // 无值标志:help/version 任意位置优先
        if (!positionalOnly && (arg == "-h" || arg == "--help")) {
            out.helpFlag = true;
            continue;
        }
        if (!positionalOnly && (arg == "-V" || arg == "--version")) {
            out.versionFlag = true;
            continue;
        }
        if (!positionalOnly && arg == "--json") {
            out.jsonFlag = true;
            continue;
        }
        if (!positionalOnly && arg == "--deep") {
            out.deepFlag = true;
            continue;
        }
        // 带值标志:短形式 "-s X" 与长形式 "--serial X"
        if (!positionalOnly && (arg == "-s" || arg == "--serial")) {
            if (i + 1 >= argc) {
                error = "选项 " + arg + " 缺少参数";
                return 1;
            }
            out.serial = argv[++i];
            continue;
        }
        if (!positionalOnly && (arg == "-o" || arg == "--output")) {
            if (i + 1 >= argc) {
                error = "选项 " + arg + " 缺少参数";
                return 1;
            }
            out.outputDir = argv[++i];
            continue;
        }
        // 长标志 "--flag=value" 形式
        if (!positionalOnly && arg.starts_with("--serial=")) {
            out.serial = arg.substr(9);
            continue;
        }
        if (!positionalOnly && arg.starts_with("--output=")) {
            out.outputDir = arg.substr(9);
            continue;
        }
        // 其他 "-" 开头一律视为未知选项
        if (!positionalOnly && arg.starts_with("-") && arg.size() > 1) {
            error = "未知选项: " + arg;
            return 1;
        }
        // 位置参数:第一个是子命令,其余收集
        if (out.command.empty()) {
            out.command = arg;
        } else {
            out.positional.push_back(arg);
        }
    }
    return 0;
}

}  // namespace util
