#include "view.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

namespace {

// 计算字符串的终端显示宽度:中文按2列计算,并跳过 ANSI 颜色转义序列
size_t DisplayWidth(const std::string& text) {
    size_t width = 0;
    for (size_t i = 0; i < text.size();) {
        unsigned char c = static_cast<unsigned char>(text[i]);
        if (c == 0x1B) {  // 跳过 ANSI 颜色转义
            size_t j = i + 1;
            while (j < text.size() && text[j] != 'm') ++j;
            i = (j < text.size()) ? j + 1 : text.size();
            continue;
        }
        if (c < 0x80) {
            ++width;
            ++i;
        } else {
            width += 2;
            i += (c >= 0xF0) ? 4 : (c >= 0xE0) ? 3 : (c >= 0xC0) ? 2 : 1;
        }
    }
    return width;
}

// 绘制带标题的信息面板:粉色标题边框 + 青色内容边框
void DrawPanel(const std::string& title,
               const std::vector<std::string>& contentLines) {
    constexpr size_t kPanelWidth = 60;

    // 顶部边框: +==[ 标题 ]====...====+
    std::string topBorder = "+==[ " + title + " ]";
    size_t usedWidth = DisplayWidth(topBorder);
    if (usedWidth < kPanelWidth - 1)
        topBorder += std::string(kPanelWidth - 1 - usedWidth, '=');
    topBorder += "+";
    std::cout << View::PINK << topBorder << View::WHITE << "\n";

    // 内容行: | 内容 ... |
    for (const auto& line : contentLines) {
        size_t lineWidth = DisplayWidth(line);
        size_t padding =
            (lineWidth < kPanelWidth - 3) ? kPanelWidth - 3 - lineWidth : 0;
        std::cout << View::CYAN << "|" << View::WHITE << " " << line
                  << std::string(padding, ' ') << View::CYAN << "|" << View::WHITE
                  << "\n";
    }

    // 底部边框: +----...----+
    std::cout << View::PINK << "+" << std::string(kPanelWidth - 2, '-') << "+"
              << View::WHITE << "\n\n";
}

}  // namespace

// 启动横幅:ASCII 手机图形 + 取证终端外壳信息面板
void View::DisplayHeader() {
    // ASCII 手机图形(青色线条与屏幕)
    std::cout << CYAN << R"(
       _______________
      /               \
     /                 \
    |  _______________  |
    | |               | |
    | |   ........    | |
    | |   . PHONE .   | |
    | |   ........    | |
    | |               | |
    | |_______________| |
    |                   |
     \                 /
      \_______________/
)" << WHITE;

    // 标题与系统状态
    std::cout << PINK << BOLD << "\n  LQZ FORENSIC SHELL / 灵取证" << WHITE
              << "\n";
    std::cout << DIM
              << "  [TARGET] android device    [MODE] forensic extraction    "
                 "[BUILD] v1.0.0"
              << WHITE << "\n";
    std::cout << PINK
              << "  ~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~"
              << WHITE << "\n\n";

    DrawPanel("法律依据 LEGAL BASIS",
              {"  • 《中华人民共和国刑事诉讼法》第138条",
               "    - 关于电子数据收集与使用的规定",
               "  • 《中华人民共和国网络安全法》第50条",
               "    - 网络运营者配合网络安全与信息内容管理",
               "  • 《中华人民共和国公安机关办理刑事案件程序规定》第234条",
               "    - 电子数据取证程序与要求",
               "  • 《中华人民共和国电子签名法》第14条",
               "    - 电子签名的法律效力",
               "  • 《电子数据取证规范》GA/T 1070-2013",
               "    - 移动设备取证技术标准",
               "  • 《移动互联网应用程序信息安全管理规定》",
               "    - APP数据安全与隐私保护要求"});

    DrawPanel("警告事项 WARNING",
              {"  • 未经授权的数据提取可能构成违法行为",
               "    - 可能违反《刑法》第285条",
               "    - 可能违反《个人信息保护法》相关规定",
               "  • 提取的数据可能涉及个人隐私，请依法处理",
               "    - 需遵守数据保密原则",
               "    - 确保数据存储安全",
               "  • 建议在获得合法授权后再进行数据提取",
               "    - 获取书面授权文件",
               "    - 记录授权过程",
               "  • 不当使用可能承担法律责任",
               "    - 民事赔偿责任",
               "    - 行政处罚责任",
               "    - 刑事责任"});

    DrawPanel("免责声明 DISCLAIMER",
              {"  • 本工具仅供执法部门在法律框架内使用",
               "  • 作者对任何非法使用或滥用概不负责",
               "  • 使用本工具即表示同意遵守相关法律法规",
               "  • 请确保在合法合规的前提下使用本工具"});

    DrawPanel("开发信息 DEVELOPER",
              {"  作者 ...... 钟智强",
               "  电邮 ...... johnmelodymel@qq.com"});
}

// 模块菜单:以面板形式列出全部功能与命令
void View::DisplayHelp() {
    DrawPanel("MODULES 功能模块",
              {"\033[95m[01]\033[0m Call Logs       通话记录",
               "\033[95m[02]\033[0m SMS             短信",
               "\033[95m[03]\033[0m Photos          照片",
               "\033[95m[04]\033[0m Videos          视频",
               "\033[95m[05]\033[0m APKs            应用",
               "\033[95m[06]\033[0m Upload          上传文件",
               "\033[95m[07]\033[0m Documents       文档",
               "\033[95m[08]\033[0m Sensitive Info  敏感信息"});

    DrawPanel("SYSTEM 系统信息",
              {"\033[95m[09]\033[0m  WiFi Info       获取WiFi信息",
               "\033[95m[10]\033[0m System Info      系统信息",
               "\033[95m[11]\033[0m Packages         应用列表",
               "\033[95m[12]\033[0m WhatsApp         提取WhatsApp",
               "\033[95m[13]\033[0m Browser          浏览器数据",
               "\033[95m[14]\033[0m Notifications    通知记录",
               "\033[95m[15]\033[0m WeChat           提取微信数据"});

    DrawPanel("COMMANDS 常用命令",
              {"\033[96m1-15\033[0m    execute feature    执行对应功能",
               "\033[96mhelp/?\033[0m  show help           显示帮助",
               "\033[96mabout\033[0m   show about          关于程序",
               "\033[96mclear\033[0m   clear screen        清屏",
               "\033[96mexit/0\033[0m  exit program        退出程序"});
}

// 设备状态面板:连接成功与未检测到两种情况
void View::ShowDeviceStatus(bool isDeviceConnected,
                            const std::string& deviceModelName) {
    if (isDeviceConnected) {
        std::cout << GREEN << "\n  [+] device online" << WHITE << "\n";
        DrawPanel("DEVICE STATUS 设备状态",
                  {"\033[92m  [*] 设备已连接 Device Connected\033[0m",
                   "  model ........ " + deviceModelName});
    } else {
        std::cout << RED << "\n  [!] no device detected" << WHITE << "\n";
        DrawPanel("DEVICE STATUS 设备状态",
                  {"\033[91m  [!] 未检测到设备 No Device\033[0m",
                   "  hint ......... 检查 USB 连接与 ADB 授权"});
    }
}

// 任务进度条:青色任务标记 + 粉色进度槽
void View::ShowProgress(const std::string& currentOperationName,
                        int completionPercentage) {
    std::cout << CYAN << "\r  [*] " << currentOperationName << " " << WHITE;
    std::cout << PINK << "[";
    int progressBarFilledLength = 40 * completionPercentage / 100;
    for (int progressBarCharacterIndex = 0; progressBarCharacterIndex < 40;
         ++progressBarCharacterIndex) {
        if (progressBarCharacterIndex < progressBarFilledLength)
            std::cout << "=";
        else if (progressBarCharacterIndex == progressBarFilledLength)
            std::cout << ">";
        else
            std::cout << " ";
    }
    std::cout << "] " << std::setw(3) << completionPercentage << "%" << WHITE
              << std::flush;
    if (completionPercentage == 100) std::cout << "\n";
}

// 错误提示,红色 [err] 标签
void View::ShowError(const std::string& displayMessage) {
    std::cout << RED << "  [err] " << displayMessage << WHITE << "\n";
}

// 成功提示,绿色 [ ok ] 标签
void View::ShowSuccess(const std::string& displayMessage) {
    std::cout << GREEN << "  [ ok ] " << displayMessage << WHITE << "\n";
}

// 普通提示,按指定颜色打印 [ * ] 标签
void View::ShowMessage(const std::string& displayMessage,
                       const std::string& colorCode) {
    std::cout << colorCode << "  [ * ] " << displayMessage << WHITE << "\n";
}

// 获取用户输入,粉色 >> 提示符
void View::GetInput(const std::string& promptMessage,
                    std::string& userInputBuffer) {
    std::cout << PINK << promptMessage << WHITE;
    std::getline(std::cin, userInputBuffer);
}

// 清空屏幕
void View::ClearScreen() {
#ifdef _WIN32
    system("cls");  // Windows系统下的清屏魔法~
#else
    system("clear");  // Unix系统下的清屏魔法~
#endif
}

// 获取当前操作系统类型
std::string View::GetOSType() {
#ifdef _WIN32
    return "Windows";  // Windows系统就返回这个~
#elif __APPLE__
    return "MacOS";  // 苹果系统就返回这个~
#elif __linux__
    return "Linux";  // Linux系统就返回这个~
#elif __unix__
    return "Unix";  // Unix系统就返回这个~
#else
    return "Unknown OS";  // 不认识的系统就这样说啦~
#endif
}