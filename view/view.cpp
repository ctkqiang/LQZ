#include "view.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

const std::string View::RED = "\033[31m";
const std::string View::BLUE = "\033[34m";
const std::string View::GREEN = "\033[32m";
const std::string View::YELLOW = "\033[33m";
const std::string View::WHITE = "\033[0m";
const std::string View::CYAN = "\033[36m";

void View::DisplayHeader() {
    std::cout << BLUE << CYAN << R"(
       _______________
      /               \
     /                 \
    |  _______________  |
    | |               | |
    | |   \033[37m████████\033[36m    | |
    | |   \033[37m█  \033[36mPHONE \033[37m█\033[36m  | |
    | |   \033[37m████████\033[36m    | |
    | |               | |
    | |_______________| |
    |        [ ]        |
     \                 /
      \_______________/

    ╔══════════════════════════════════════════════════════════╗
    ║     Android 手机取证工具 v1.0.0                            
    ║     (安卓数据提取与分析系统)                                 
    ╠══════════════════════════════════════════════════════════╣
    ║  法律依据:                                                
    ║  • 《中华人民共和国刑事诉讼法》第138条                        
    ║  • 《中华人民共和国网络安全法》第50条                         
    ║  • 《电子数据取证规范》GA/T 1070-2013                       
    ╠══════════════════════════════════════════════════════════╣
    ║  免责声明:                                               
    ║  • 本工具仅供执法部门在法律框架内使用                          
    ║  • 作者对任何非法使用或滥用概不负责                           
    ║  • 使用本工具即表示同意遵守相关法律法规                        
    ╠══════════════════════════════════════════════════════════╣
    ║  开发信息:                                                
    ║  作者: 钟智强                                              
    ╚══════════════════════════════════════════════════════════╝
)" << WHITE << std::endl;
}

void View::DisplayHelp() {
    std::cout << BLUE << "\n[x] Features/功能:" << WHITE << std::endl;
    std::cout << BLUE << "[1] " << WHITE << "Dump Call Logs   导出通话记录"
              << std::endl;
    std::cout << BLUE << "[2] " << WHITE << "Dump SMS         导出短信"
              << std::endl;
    std::cout << BLUE << "[3] " << WHITE << "Dump Photos      导出照片"
              << std::endl;
    std::cout << BLUE << "[4] " << WHITE << "Dump Videos      导出视频"
              << std::endl;
    std::cout << BLUE << "[5] " << WHITE << "Dump APKs        导出应用"
              << std::endl;
    std::cout << BLUE << "[6] " << WHITE << "Upload Files     上传文件"
              << std::endl;
    std::cout << BLUE << "[7] " << WHITE << "Dump Documents   导出文档"
              << std::endl;
    std::cout << BLUE << "[8] " << WHITE << "Extract Info     提取敏感信息"
              << std::endl;

    std::cout << BLUE << "[9] " << WHITE << "WiFi Info       获取WiFi信息"
              << std::endl;
    std::cout << BLUE << "[10] " << WHITE << "System Info    系统信息"
              << std::endl;
    std::cout << BLUE << "[11] " << WHITE << "List Packages  应用列表"
              << std::endl;
    std::cout << BLUE << "[12] " << WHITE << "WhatsApp Data  提取WhatsApp"
              << std::endl;
    std::cout << BLUE << "[13] " << WHITE << "Browser Data   浏览器数据"
              << std::endl;
    std::cout << BLUE << "[14] " << WHITE << "Notifications  通知记录"
              << std::endl;

    std::cout << GREEN << "\n[x] Commands/命令:" << WHITE << std::endl;
    std::cout << "1-8: Execute feature   执行相应功能" << std::endl;
    std::cout << "help/?:  Show help     显示帮助" << std::endl;
    std::cout << "about:   Show about    显示关于此程序" << std::endl;
    std::cout << "clear:   Clear screen  清屏" << std::endl;
    std::cout << "exit/0:  Exit program  退出程序" << std::endl;
}

void View::ShowDeviceStatus(bool connected, const std::string& model) {
    std::cout << YELLOW << "\n[-] Connected Device/已连接设备:" << WHITE
              << std::endl;
    if (connected) {
        std::cout << GREEN << "[+] " << model << WHITE << std::endl;
    } else {
        std::cout << RED << "[!] No mobile phone detected/未检测到手机设备"
                  << WHITE << std::endl;
    }
}

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

void View::ShowError(const std::string& message) {
    std::cout << RED << "[!] Error/错误: " << message << WHITE << std::endl;
}

void View::ShowSuccess(const std::string& message) {
    std::cout << GREEN << "[+] Success/成功: " << message << WHITE << std::endl;
}

void View::ShowMessage(const std::string& message, const std::string& color) {
    std::cout << color << "[*] " << message << WHITE << std::endl;
}

void View::GetInput(const std::string& prompt, std::string& input) {
    std::cout << YELLOW << prompt << WHITE;
    std::getline(std::cin, input);
}

void View::ClearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

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