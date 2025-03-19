#include "view.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

// 小草莓红：用来标记重要错误信息的颜色哟~(✿◠‿◠)
const std::string View::RED = "\033[31m";
// 天空蓝：用来显示操作提示的清爽颜色哒~(๑•. •๑)
const std::string View::BLUE = "\033[34m";
// 薄荷绿：成功时闪烁的治愈系颜色呀~(๑•ω•๑)
const std::string View::GREEN = "\033[32m";
// 柠檬黄：温馨提示的可爱颜色哟~(✿◠‿◠)
const std::string View::YELLOW = "\033[33m";
// 云朵白：默认的干净纯洁颜色哒~(๑•. •๑)
const std::string View::WHITE = "\033[0m";
// 水晶蓝：系统信息展示的高级感颜色呀~(๑•ω•๑)
const std::string View::CYAN = "\033[36m";

// 小司仪登场~展示欢迎标题页的魔法仪式哟~(✿◠‿◠)
// 会用闪烁的星星特效和可爱 ASCII 艺术欢迎用户哒~
void View::DisplayHeader() {
    std::cout << BLUE << CYAN << R"(
       ✿ _______________
      /  ∗            ∗ \
     /   ⋆             ⋆ \
    |  ✧_______________✧  |
    | |  ⋆           ⋆  | |
    | |   \033[37m♡♡♡♡♡♡♡♡\033[36m    | |
    | |   \033[37m♡ \033[36mPHONE \033[37m♡\033[36m  | |
    | |   \033[37m♡♡♡♡♡♡♡♡\033[36m    | |
    | |  ⋆           ⋆  | |
    | |_______________| |
    |      ✿[♡]✿       |
     \   ⋆         ⋆   /
      \_______________/

    ╭══════════════════════════════════════════════════════════╮
    │     ✧ Android 手机取证工具 v1.0.0 ✧                        
    │     (安卓数据提取与分析系统)                                 
    ╰──────────────────────────────────────────────────────────╯
    ╭══════════════════════════════════════════════════════════╮
    │  法律依据 ♡                                                
    │  • 《中华人民共和国刑事诉讼法》第138条                        
    │  • 《中华人民共和国网络安全法》第50条                         
    │  • 《电子数据取证规范》GA/T 1070-2013                       
    ╰──────────────────────────────────────────────────────────╯
    ╭══════════════════════════════════════════════════════════╮
    │  免责声明 ♡                                               
    │  • 本工具仅供执法部门在法律框架内使用                          
    │  • 作者对任何非法使用或滥用概不负责                           
    │  • 使用本工具即表示同意遵守相关法律法规                        
    ╰──────────────────────────────────────────────────────────╯
    ╭══════════════════════════════════════════════════════════╮
    │  开发信息 ♡                                                
    │  作者: 钟智强                                              
    ╰──────────────────────────────────────────────────────────╯
)" << WHITE << std::endl;
}

// 小百科全书在此~展示所有可用命令的魔法手册哟~(๑•. •๑)
// 会用彩虹色文字列出各种操作指令，超详细哒~
void View::DisplayHelp() {
    // Title with decorative border
    std::cout << CYAN << "\n╭────────── ♡ 功能菜单 Features ♡ ──────────╮"
              << WHITE << std::endl;

    // Data Extraction Section
    std::cout << BLUE << "\n✧ 数据提取 Data Extraction ✧" << WHITE << std::endl;
    std::cout << BLUE << "  [1] " << WHITE << "📱 Dump Call Logs   导出通话记录"
              << std::endl;
    std::cout << BLUE << "  [2] " << WHITE << "💌 Dump SMS         导出短信"
              << std::endl;
    std::cout << BLUE << "  [3] " << WHITE << "📸 Dump Photos      导出照片"
              << std::endl;
    std::cout << BLUE << "  [4] " << WHITE << "🎥 Dump Videos      导出视频"
              << std::endl;
    std::cout << BLUE << "  [5] " << WHITE << "📦 Dump APKs        导出应用"
              << std::endl;
    std::cout << BLUE << "  [6] " << WHITE << "📤 Upload Files     上传文件"
              << std::endl;
    std::cout << BLUE << "  [7] " << WHITE << "📑 Dump Documents   导出文档"
              << std::endl;
    std::cout << BLUE << "  [8] " << WHITE << "🔍 Extract Info     提取敏感信息"
              << std::endl;

    // System Information Section
    std::cout << BLUE << "\n✧ 系统信息 System Info ✧" << WHITE << std::endl;
    std::cout << BLUE << "  [9]  " << WHITE << "📶 WiFi Info       获取WiFi信息"
              << std::endl;
    std::cout << BLUE << "  [10] " << WHITE << "⚙️  System Info    系统信息"
              << std::endl;
    std::cout << BLUE << "  [11] " << WHITE << "📱 List Packages  应用列表"
              << std::endl;
    std::cout << BLUE << "  [12] " << WHITE << "💬 WhatsApp Data  提取WhatsApp"
              << std::endl;
    std::cout << BLUE << "  [13] " << WHITE << "🌐 Browser Data   浏览器数据"
              << std::endl;
    std::cout << BLUE << "  [14] " << WHITE << "🔔 Notifications  通知记录"
              << std::endl;

    // Commands Section
    std::cout << GREEN << "\n✧ 常用命令 Commands ✧" << WHITE << std::endl;
    std::cout << YELLOW << "  ⭐ " << WHITE
              << "1-14: Execute feature  执行对应功能" << std::endl;
    std::cout << YELLOW << "  ⭐ " << WHITE << "help/?:  Show help    显示帮助"
              << std::endl;
    std::cout << YELLOW << "  ⭐ " << WHITE << "about:   Show about   关于程序"
              << std::endl;
    std::cout << YELLOW << "  ⭐ " << WHITE << "clear:   Clear screen 清屏"
              << std::endl;
    std::cout << YELLOW << "  ⭐ " << WHITE << "exit/0:  Exit program 退出程序"
              << std::endl;

    // Bottom border
    std::cout << CYAN << "\n╰─────────────────────────────────────────╯"
              << WHITE << std::endl;
}

// 小雷达启动~显示设备连接状态的魔法看板哟~(๑•ω•๑)
// connected 参数是是否连接的小开关，model 是设备型号小标签
void View::ShowDeviceStatus(bool connected, const std::string& model) {
    std::cout << YELLOW << "\n[-] Connected Device/已连接设备:" << WHITE
              << std::endl;
    if (connected) {
        std::cout << GREEN << "[+] " << model << WHITE << std::endl;
    } else {
        std::cout << RED << "[!] No mobile phone detected/未检测到手机设备"
                  << WHITE << std::endl;
    }
    // Decorative border for device status
    std::cout << CYAN << "\n╭──── Device Status 设备状态 ────╮" << WHITE
              << std::endl;
    if (connected) {
        std::cout << GREEN << "  ✓ 设备已连接 Device Connected" << std::endl;
        std::cout << "  📱 型号 Model: " << model << WHITE << std::endl;
    } else {
        std::cout << RED << "  ✗ 未检测到设备 No Device" << std::endl;
        std::cout << "  💡 请检查连接 Check Connection" << WHITE << std::endl;
    }
    std::cout << CYAN << "╰────────────────────────────╯" << WHITE << std::endl;
}

// 进度条小管家~显示操作进度的魔法仪表盘哟~(✿◠‿◠)
// operation 是当前操作的小名称，percentage 是完成度小数值
void View::ShowProgress(const std::string& operation, int percentage) {
    std::cout << GREEN << "\r[*] " << operation << " [";
    int pos = 50 * percentage / 100;
    for (int i = 0; i < 50; ++i) {
        if (i < pos)
            std::cout << "=";
        else if (i == pos)
            std::cout << ">";
        else
            std::cout << " ";
    }
    std::cout << "] " << std::setw(3) << percentage << "%" << std::flush;
    if (percentage == 100) std::cout << std::endl;
}

// 错误警报器~用红色闪烁文字报告错误的魔法喇叭哟~(๑•. •๑)
// message 是要显示的错误小提示，会用可爱的语气读出来哒
void View::ShowError(const std::string& message) {
    std::cout << RED << "[!] Error/错误: " << message << WHITE << std::endl;
}

// 成功小烟花~用绿色星星特效庆祝成功的魔法礼炮哟~(๑•ω•๑)
// message 是成功后的小庆祝语，会有 sparkles 音效哒
void View::ShowSuccess(const std::string& message) {
    std::cout << GREEN << "[+] Success/成功: " << message << WHITE << std::endl;
}

// 彩色话痨模式~用指定颜色显示自定义信息的魔法泡泡哟~(✿◠‿◠)
// message 是要说的话，color 是选择的颜色魔法标记
void View::ShowMessage(const std::string& message, const std::string& color) {
    std::cout << color << "[*] " << message << WHITE << std::endl;
}

// 小话筒启动~获取用户输入的魔法吸管哟~(๑•. •๑)
// prompt 是提示语小泡泡，input 是收集输入的小盒子
void View::GetInput(const std::string& prompt, std::string& input) {
    std::cout << YELLOW << prompt << WHITE;
    std::getline(std::cin, input);
}

// 橡皮擦魔法~清空屏幕的神奇咒语哟~(๑•ω•๑)
// 会把屏幕擦得像新纸一样干净，还会有 "poof" 音效哒
void View::ClearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// 小侦探技能~获取当前操作系统类型的魔法扫描仪哟~(✿◠‿◠)
// 会返回 "Windows"、"Mac" 或 "Linux" 等小标签
std::string View::GetOSType() {
#ifdef _WIN32
    return "Windows";
#elif __APPLE__
    return "MacOS";
#elif __linux__
    return "Linux";
#elif __unix__
    return "Unix";
#else
    return "Unknown OS";
#endif
}
