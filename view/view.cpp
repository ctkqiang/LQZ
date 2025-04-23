#include "view.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

// 定义各种可爱的颜色魔法~
const std::string View::RED = "\033[31m";  // 小草莓红色，用来提醒重要事项哟~
const std::string View::BLUE = "\033[34m";  // 天空蓝色，像清爽的海洋一样~
const std::string View::GREEN = "\033[32m";  // 嫩芽绿色，代表希望和成功呢~
const std::string View::YELLOW = "\033[33m";  // 向日葵黄色，温暖又活泼哒~
const std::string View::WHITE = "\033[0m";  // 纯净白色，像小棉花一样柔软~
const std::string View::CYAN = "\033[36m";  // 梦幻蓝色，充满神秘感呢~

// 欢迎界面的小仙女~用漂亮的ASCII艺术来装饰哟~
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
    │    - 关于电子数据收集与使用的规定                            
    │  • 《中华人民共和国网络安全法》第50条                         
    │    - 网络运营者配合网络安全与信息内容管理                     
    │  • 《中华人民共和国公安机关办理刑事案件程序规定》第234条         
    │    - 电子数据取证程序与要求                                
    │  • 《中华人民共和国电子签名法》第14条                         
    │    - 电子签名的法律效力                                    
    │  • 《电子数据取证规范》GA/T 1070-2013                       
    │    - 移动设备取证技术标准                                  
    │  • 《移动互联网应用程序信息安全管理规定》                      
    │    - APP数据安全与隐私保护要求                             
    ╰──────────────────────────────────────────────────────────╯
    ╭══════════════════════════════════════════════════════════╮
    │  警告事项 ⚠                                               
    │  • 未经授权的数据提取可能构成违法行为                         
    │    - 可能违反《刑法》第285条                               
    │    - 可能违反《个人信息保护法》相关规定                       
    │  • 提取的数据可能涉及个人隐私，请依法处理                      
    │    - 需遵守数据保密原则                                    
    │    - 确保数据存储安全                                      
    │  • 建议在获得合法授权后再进行数据提取                         
    │    - 获取书面授权文件                                      
    │    - 记录授权过程                                         
    │  • 不当使用可能承担法律责任                                 
    │    - 民事赔偿责任                                         
    │    - 行政处罚责任                                         
    │    - 刑事责任                                            
    ╰──────────────────────────────────────────────────────────╯
    ╭══════════════════════════════════════════════════════════╮
    │  免责声明 ♡                                               
    │  • 本工具仅供执法部门在法律框架内使用                          
    │  • 作者对任何非法使用或滥用概不负责                           
    │  • 使用本工具即表示同意遵守相关法律法规                        
    │  • 请确保在合法合规的前提下使用本工具                         
    ╰──────────────────────────────────────────────────────────╯
    ╭══════════════════════════════════════════════════════════╮
    │  开发信息 ♡                                                
    │  作者: 钟智强
    |  电邮：johnmelodymel@qq.com                                                  
    ╰──────────────────────────────────────────────────────────╯
)" << WHITE << std::endl;
}

// 小百科全书在此~展示所有可用命令的魔法手册哟~(๑•. •๑)
// 会用彩虹色文字列出各种操作指令，超详细哒~
void View::DisplayHelp() {
    // 漂亮的菜单边框~
    std::cout << CYAN << "\n╭────────── ♡ 功能菜单 Features ♡ ──────────╮"
              << WHITE << std::endl;

    // 数据提取功能区域~
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

    // 系统信息功能区域~
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
    std::cout << BLUE << "  [15] " << WHITE << "💭 WeChat Data    提取微信数据"
              << std::endl;

    // 常用命令区域~
    std::cout << GREEN << "\n✧ 常用命令 Commands ✧" << WHITE << std::endl;
    std::cout << YELLOW << "  ⭐ " << WHITE
              << "1-15: Execute feature  执行对应功能" << std::endl;
    std::cout << YELLOW << "  ⭐ " << WHITE << "help/?:  Show help    显示帮助"
              << std::endl;
    std::cout << YELLOW << "  ⭐ " << WHITE << "about:   Show about   关于程序"
              << std::endl;
    std::cout << YELLOW << "  ⭐ " << WHITE << "clear:   Clear screen 清屏"
              << std::endl;
    std::cout << YELLOW << "  ⭐ " << WHITE << "exit/0:  Exit program 退出程序"
              << std::endl;

    // 底部装饰边框~
    std::cout << CYAN << "\n╰─────────────────────────────────────────╯"
              << WHITE << std::endl;
}

// 设备状态小助手~随时汇报设备连接情况哟~
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

// 进度条小精灵~用可爱的方式显示任务进度呢~
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

// 错误提示小天使~用温柔的方式告诉用户出错啦~
void View::ShowError(const std::string& message) {
    std::cout << RED << "[!] Error/错误: " << message << WHITE << std::endl;
}

// 成功提示小仙子~开心地庆祝任务完成啦~
void View::ShowSuccess(const std::string& message) {
    std::cout << GREEN << "[+] Success/成功: " << message << WHITE << std::endl;
}

// 消息传递小精灵~用漂亮的颜色显示各种提示哟~
void View::ShowMessage(const std::string& message, const std::string& color) {
    std::cout << color << "[*] " << message << WHITE << std::endl;
}

// 用户输入小助手~温柔地等待用户的指令呢~
void View::GetInput(const std::string& prompt, std::string& input) {
    std::cout << YELLOW << prompt << WHITE;
    std::getline(std::cin, input);
}

// 清屏幕小魔法师~让界面保持整洁干净呢~
void View::ClearScreen() {
#ifdef _WIN32
    system("cls");  // Windows系统下的清屏魔法~
#else
    system("clear");  // Unix系统下的清屏魔法~
#endif
}

// 系统检测小能手~帮忙识别当前运行的系统类型哟~
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