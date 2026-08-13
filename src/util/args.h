#ifndef UTIL_ARGS_H
#define UTIL_ARGS_H

#include <string>
#include <vector>

// 手写命令行解析器(零依赖)。
// 不用 getopt:它 POSIX 专属,Windows/MSYS2 下不可移植。
namespace util {

struct ParsedArgs {
    std::string command;     // 子命令名;空 = 交互模式
    std::string serial;      // -s / --serial
    std::string outputDir;   // -o / --output
    bool jsonFlag = false;   // --json
    bool deepFlag = false;   // --deep(verify 子命令用)
    bool helpFlag = false;   // -h / --help
    bool versionFlag = false;  // -V / --version
    std::vector<std::string> positional;  // 位置参数(如 connect <ip>)
};

// 成功返回 0;参数错误时返回非 0 并在 error 中写明原因(调用方以退出码 2 处理)
int ParseArgs(int argc, char* argv[], ParsedArgs& out, std::string& error);

}  // namespace util

#endif
