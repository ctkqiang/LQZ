// 哇哦~欢迎来到超可爱的数据模型类世界呀
// 这里就像是一个充满魔法的数据小窝窝，每一行代码都像是小仙子在翩翩起舞呢，可一定要好好疼爱每一个字节哟~
#include "model.h"

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string_view>
#include <vector>

// 呐呐~这个 `ADB_WIRELESS_CONNECT_COMMAND_TEMPLATE`
// 呀，就像是一座神奇的桥梁，它能让我们的电脑和安卓设备手牵手呢
// 不过要记得把 <device_ip> 换成设备真正的 IP
// 地址哟，不然桥梁可就搭不起来啦~
[[maybe_unused]] const std::string ADB_WIRELESS_CONNECT_COMMAND_TEMPLATE =
    "adb connect <device_ip>:5555";
// 哎呀呀，`ADB_WIRELESS_DISCONNECT_COMMAND_TEMPLATE`
// 呢，就像是一场短暂的分别，当我们不想和设备聊天啦，就用它来断开连接哒
// 同样也要把 <device_ip> 填对哟~
[[maybe_unused]] const std::string ADB_WIRELESS_DISCONNECT_COMMAND_TEMPLATE =
    "adb disconnect <device_ip>:5555";
// 嘿嘿，`ADB_PULL_COMMAND_TEMPLATE`
// 可是个超厉害的小魔法师，它能把安卓设备里的宝贝文件都拉到我们电脑里来呢
// <remote_path> 就是设备里文件藏着的地方，<local_path>
// 就是我们要把文件放到电脑的哪里哒~
[[maybe_unused]] const std::string ADB_PULL_COMMAND_TEMPLATE =
    "adb pull <remote_path> <local_path>";
// 哟哟~`ADB_SHELL_COMMAND_PREFIX`
// 就像是一把神奇的钥匙，有了它，我们就能打开安卓设备的小秘密盒子，在里面执行各种有趣的命令啦
const std::string ADB_SHELL_COMMAND_PREFIX = "adb shell ";
// 哇塞~`ADB_BACKUP_SMS_COMMAND` 这个大宝贝，能把安卓设备里的短信都小心翼翼地备份到
// `Datas/Phone_dump_sms.ab` 文件里哟，就像是给短信们找了个安全的小房子住呢
const char* ADB_BACKUP_SMS_COMMAND =
    "adb backup -noapk -f Datas/Phone_dump_sms.ab -nocompress "
    "com.android.providers.telephony";

// 呀~`MODEL_ERROR_RED_COLOR_CODE`
// 就像是一个调皮的小精灵，它能让我们输出的文字变成超级显眼的红色呢，这样重要的信息就一下子跳出来啦
const std::string MODEL_ERROR_RED_COLOR_CODE = "\033[31m";
// 嘿嘿，`MODEL_DEFAULT_WHITE_COLOR_CODE`
// 则是一个温柔的小天使，它能让文字颜色乖乖地变回白色，让一切都恢复到原来的可爱模样哒
const std::string MODEL_DEFAULT_WHITE_COLOR_CODE = "\033[0m";

// 哇哦~这个 `GetDeviceModel`
// 函数呀，就像是一个超级侦探，它能帮我们找出连接的安卓设备到底是什么型号哟
// 调用它，就能揭开设备型号的神秘面纱啦~
std::string Model::GetDeviceModel() const { return connectedDeviceModelName; }

// 呐呐~`Shell` 函数就像是一个贴心的小秘书，我们把要执行的 ADB shell
// 命令告诉它，它就会乖乖地去帮我们办好，还会把执行的结果打印出来给我们看呢
void Model::Shell(const std::string &shellCommandToExecute) {
    // 先让 `executeCommand` 这个小助手去执行 ADB_SHELL
    // 加上我们给的具体命令的组合
    std::string commandExecutionOutput =
        executeCommand(ADB_SHELL_COMMAND_PREFIX + shellCommandToExecute);
    // 然后把执行的结果美美的打印出来哟
    printf("%s\n", commandExecutionOutput.c_str());
}

// 哎呀呀，`executeCommand`
// 函数可是个超级能干的小魔法师呢，它能执行系统命令，还能把命令执行后的输出结果都收集起来给我们哟
std::string Model::executeCommand(
    const std::string &shellCommandToExecute) {
    // 这个 `commandOutputLineBuffer` 就像是一个小口袋，用来暂时装一下命令输出的内容哒
    char commandOutputLineBuffer[128];
    // `accumulatedCommandOutput`
    // 则是一个大箱子，把口袋里的内容都收集起来，最后变成完整的输出结果哟
    std::string accumulatedCommandOutput;
    // 打开一个神奇的管道，让命令能在里面运行呢
    FILE* commandOutputPipe = popen(shellCommandToExecute.c_str(), "r");

    // 如果管道打开失败了，就像魔法失灵啦，只能返回一个空空的箱子咯
    if (!commandOutputPipe) return "";

    // 从管道里把输出内容一点点地掏出来，放到大箱子里哟
    while (fgets(commandOutputLineBuffer, sizeof(commandOutputLineBuffer),
                 commandOutputPipe) != nullptr) {
        accumulatedCommandOutput += commandOutputLineBuffer;
    }

    // 用完管道后，要把它乖乖地关上哟
    pclose(commandOutputPipe);

    // 最后把装满输出结果的大箱子交还给我们啦
    return accumulatedCommandOutput;
}

// 哇塞~`CheckDevice`
// 函数就像是一个小门卫，它能帮我们看看有没有成功连接到安卓设备呢
// 如果连接上了，还会把设备型号偷偷记下来哟~
bool Model::CheckDevice() {
    // 先让 `executeCommand` 小助手去执行 `adb devices -l`
    // 命令，看看能得到什么信息
    std::string adbDevicesListOutput = executeCommand("adb devices -l");

    // 如果输出里找到了 "model" 这个小线索，说明可能已经连接到设备啦
    if (adbDevicesListOutput.find("model") != std::string::npos) {
        // 用一个神奇的正则表达式魔法，把设备型号从输出里抓出来
        std::regex deviceModelRegexPattern("device:[a-z0-9]+");
        // 这个 `deviceModelRegexMatch` 就像是一个小夹子，用来夹住抓到的设备型号
        std::smatch deviceModelRegexMatch;

        // 如果成功夹住了设备型号，就把它放到
        // `connectedDeviceModelName` 这个小盒子里保存起来，然后告诉我们连接成功啦
        if (std::regex_search(adbDevicesListOutput, deviceModelRegexMatch,
                              deviceModelRegexPattern)) {
            connectedDeviceModelName = deviceModelRegexMatch.str();
            return true;
        }
    }

    // 如果没有抓到设备型号，那就说明还没连接上设备哟
    return false;
}

// 哟哟~`DumpSMS` 函数就像是一个超级细心的小管家，它会先给我们创建一个 `Datas`
// 小房间，然后把安卓设备里的短信都好好地备份起来呢
void Model::DumpSMS() {
    // 轻轻地创建一个 `Datas` 小房间，如果房间已经有啦，也没关系哒
    std::filesystem::create_directories("Datas");

    // 用 `ADB_BACKUP_SMS_COMMAND` 这个大宝贝把短信备份好
    system(ADB_BACKUP_SMS_COMMAND);
    // 再用 `mvt - android`
    // 工具检查备份文件，把有用的数据提取出来，最后把备份文件这个小包袱扔掉哟
    system(
        "mvt-android check-backup Datas/Phone_dump_sms.ab -o Datas/ && rm "
        "Datas/Phone_dump_sms.ab");
}

// 哇哦~`DumpPhotos` 函数就像是一个爱拍照的小天使，它会创建一个 `Datas`
// 小相册，然后把安卓设备里的照片都拉到相册里来呢
void Model::DumpPhotos() {
    // 先创建一个美美的 `Datas` 小相册
    std::filesystem::create_directories("Datas");
    // 把设备里 `/sdcard/Pictures` 这个照片小仓库里的照片都搬到相册里来哟
    system("adb pull /sdcard/Pictures Datas/");
}

// 哎呀呀，`DumpDownloads` 函数就像是一个勤劳的小快递员，它会创建一个 `Datas`
// 小包裹站，然后把安卓设备里下载的文件都送到包裹站里呢
void Model::DumpDownloads() {
    // 建一个可爱的 `Datas` 小包裹站
    std::filesystem::create_directories("Datas");
    // 把设备里 `/sdcard/Download` 这个下载小基地里的文件都拉到包裹站里哟
    system("adb pull /sdcard/Download Datas/");
}

// 呐呐~`CheckAdb` 函数就像是一个小医生，它能帮我们检查一下电脑里有没有安装 ADB
// 工具呢 如果没有安装，还会根据不同的系统给我们想办法安装哟~
void Model::CheckAdb() {
    // 先让 `executeCommand` 小助手去执行 `adb version` 命令，看看能不能得到 ADB
    // 工具的版本信息
    std::string adbVersionCheckOutput = executeCommand("adb version");

    // 如果输出是空的，就说明 ADB 工具还没安装呢
    if (adbVersionCheckOutput.empty()) {
#ifdef _WIN32
        // 如果是 Windows 系统，就像小医生拿出了一个 Windows
        // 专属的小药方，提示我们正在安装 ADB 工具，然后用 `winget` 来安装
        std::cout << "ADB未安装，正在安装..." << std::endl;
        system("powershell -Command \"winget install Google.PlatformTools\"");

        // 找到用户的主目录，就像找到了小房子的地址
        std::string userHomeDirectoryPath = getenv("USERPROFILE");
        // 再找到 ADB 工具安装的地方，就像找到了小宝贝藏在哪里
        std::string androidPlatformToolsDirectoryPath =
            userHomeDirectoryPath +
            "\\AppData\\Local\\Android\\Sdk\\platform-tools";
        // 然后把 ADB
        // 工具的地址加到系统的环境变量里，就像给小宝贝在系统里留了个小纸条
        std::string setxEnvironmentVariableCommand =
            "setx PATH \"%PATH%;" + androidPlatformToolsDirectoryPath + "\"";
        // 执行这个命令，把小纸条贴好
        system(setxEnvironmentVariableCommand.c_str());
#elif __APPLE__
        // 如果是 macOS 系统，小医生就拿出了 macOS
        // 专属的小药方，提示我们正在安装 ADB 工具，然后用 `brew` 来安装
        std::cout << "ADB未安装，正在安装..." << std::endl;
        system("brew install android-platform-tools");
#else
        // 如果是其他系统，小医生有点没办法啦，只能提示我们手动安装 ADB
        // 工具，然后伤心地退出程序咯
        std::cout << "请手动安装ADB工具" << std::endl;
        exit(1);
#endif
        // 最后提醒我们重启终端，让环境变量这个小纸条生效哟
        std::cout << "请重启终端以使环境变量生效" << std::endl;
        exit(0);
    }
}

// 哇塞~`DumpContacts` 函数就像是一个超级记忆小能手，它会创建一个 `Datas`
// 小仓库，然后把安卓设备里联系人的数据库文件都存到仓库里呢
void Model::DumpContacts() {
    // 建一个大大的 `Datas` 小仓库
    std::filesystem::create_directories("Datas");
    // 把设备里联系人数据库文件
    // `/data/data/com.android.providers.contacts/databases/contacts2.db`
    // 搬到仓库里哟
    system(
        "adb pull "
        "/data/data/com.android.providers.contacts/databases/contacts2.db "
        "Datas/");
    // 再把通话记录数据库文件
    // `/data/data/com.android.providers.contacts/databases/calllog.db`
    // 也搬到仓库里哟
    system(
        "adb pull "
        "/data/data/com.android.providers.contacts/databases/calllog.db "
        "Datas/");
}

// 哟哟~`DumpMessage` 函数就像是一个贴心的小邮差，它会创建一个 `Datas`
// 小信箱，然后把安卓设备里短信的数据库文件都放到信箱里呢
void Model::DumpMessage() {
    // 先建一个萌萌的 `Datas` 小信箱
    std::filesystem::create_directories("Datas");
    // 把设备里短信数据库文件
    // `/data/data/com.android.providers.telephony/databases/mmssms.db`
    // 放到信箱里哟
    system(
        "adb pull "
        "/data/data/com.android.providers.telephony/databases/mmssms.db "
        "Datas/");
}

// 哇哦~`Dumpfiles` 函数就像是一个超级大搬运工，它会创建一个 `Datas/sdcard`
// 大仓库，然后把安卓设备里整个 SD 卡的内容都搬到仓库里呢
void Model::Dumpfiles() {
    // 建一个超级大的 `Datas` 大仓库
    std::filesystem::create_directories("Datas");
    // 把设备里 SD 卡的所有东西都搬到仓库里哟
    system("adb pull /sdcard/ Datas/sdcard/");
}

// 哎呀呀，`ConnectWirelessly`
// 函数就像是一个浪漫的小使者，它会让我们输入安卓设备的 IP
// 地址，然后尝试用无线的方式和设备牵起手来呢
void Model::ConnectWirelessly() {
    // 准备一个小本子，用来记录我们输入的设备 IP 地址
    std::string deviceIpAddressInput;
    // 温柔地提示我们输入设备的 IP 地址哟
    std::cout << "请输入手机IP地址: ";
    // 把我们输入的 IP 地址记到小本子上
    std::getline(std::cin, deviceIpAddressInput);

    // 用 IP 地址和端口号组成一个神奇的连接咒语
    std::string wirelessConnectionCommand =
        "adb connect " + deviceIpAddressInput + ":5555";
    // 让 `executeCommand` 小助手去念这个咒语，看看能不能和设备连接上
    std::string wirelessConnectionOutput =
        executeCommand(wirelessConnectionCommand);

    // 如果咒语生效啦，输出里找到了 "connected"
    // 这个小魔法词，就说明成功和设备连接上啦
    if (wirelessConnectionOutput.find("connected") != std::string::npos) {
        std::cout << "成功连接到设备 " << deviceIpAddressInput << std::endl;
    } else {
        // 如果咒语没生效，就温柔地告诉我们可能是哪里出了问题，让我们检查一下哟
        std::cout << "连接失败，请确保:" << std::endl;
        std::cout << "1. 手机和电脑在同一网络" << std::endl;
        std::cout << "2. 手机已启用USB调试和无线调试" << std::endl;
        std::cout << "3. IP地址正确" << std::endl;
    }
}

// 哇塞~`DumpCallLogs` 函数就像是一个超级记录员，它会创建一个 `Datas/CallLogs`
// 小本子，然后把安卓设备里的通话记录都工工整整地记到本子里呢
void Model::DumpCallLogs() {
    // 先建一个可爱的 `Datas/CallLogs` 小本子
    std::filesystem::create_directories("Datas/CallLogs");

    // 用命令把设备里的通话记录记到小本子里
    system(
        "adb shell content query --uri content://call_log/calls "
        "--projection date,number,duration,type "
        "> Datas/CallLogs/call_logs.log");

    // 再用另一个命令把通话记录变成美美的格式，也记到小本子里哟
    system(
        "adb shell \"content query --uri content://call_log/calls "
        "--projection date,number,duration,type "
        "| awk '{print strftime(\\\"%Y-%m-%d %H:%M:%S\\\", substr(\\$1, "
        "2)/1000), \\$2, \\$3, \\$4}' "
        "\" > Datas/CallLogs/formatted_call_logs.log");
}

// 哟哟~`DumpVideos` 函数就像是一个超级电影迷，它会创建一个 `Datas`
// 小电影院，然后把安卓设备里的视频文件都搬到电影院里呢
void Model::DumpVideos() {
    // 建一个大大的 `Datas` 小电影院
    std::filesystem::create_directories("Datas");
    // 把设备里相机拍摄的视频文件 `/sdcard/DCIM/Camera` 搬到电影院里哟
    system("adb pull /sdcard/DCIM/Camera Datas/Videos/");
    // 再把设备里电影文件夹里的视频文件 `/sdcard/Movies` 也搬到电影院里哟
    system("adb pull /sdcard/Movies Datas/Videos/");
}

// 哇哦~`DumpApks` 函数就像是一个超级软件收藏家，它会创建一个 `Datas/APKs`
// 小软件库，然后把安卓设备里的 APK 文件信息和文件都收集到软件库里呢
void Model::DumpApks() {
    // 建一个美美的 `Datas/APKs` 小软件库
    std::filesystem::create_directories("Datas/APKs");
    // 用命令把设备里已安装应用的包名信息记到软件库的一个小本子里
    system("adb shell pm list packages -f > Datas/APKs/installed_apps.log");
    // 把设备里 `/data/app/` 这个软件小仓库里的 APK 文件都搬到软件库里哟
    system("adb pull /data/app/ Datas/APKs/");
}

// 哎呀呀，`DumpDocuments` 函数就像是一个超级文档管理员，它会创建一个
// `Datas/Documents` 小文档室，然后把安卓设备里的文档文件都整理到文档室里呢
void Model::DumpDocuments() {
    // 建一个整洁的 `Datas/Documents` 小文档室
    std::filesystem::create_directories("Datas/Documents");
    // 把设备里文档文件夹 `/sdcard/Documents` 里的文档文件搬到文档室里哟
    system("adb pull /sdcard/Documents Datas/Documents/");
    // 再把设备里下载文件夹 `/sdcard/Download` 里的文档文件也搬到文档室里哟
    system("adb pull /sdcard/Download Datas/Documents/");
}

// 哇塞~`ExtractSensitiveInfo` 函数就像是一个超级小侦探，它会创建一个
// `Datas/Sensitive`
// 小秘密基地，然后把安卓设备里的敏感信息都偷偷地收集到基地里呢
void Model::ExtractSensitiveInfo() {
    // 建一个神秘的 `Datas/Sensitive` 小秘密基地
    std::filesystem::create_directories("Datas/Sensitive");
    // 把设备里 SD 卡的所有内容都搬到秘密基地里哟
    system("adb pull /sdcard/ Datas/Sensitive/");
}

// 哟哟~`GetWifiAddress` 函数就像是一个超级网络小专家，它会创建一个
// `Datas/Network` 小网络实验室，然后把安卓设备的 WiFi
// 地址和网络信息都好好地研究记录下来呢
void Model::GetWifiAddress() {
    // 建一个科技感满满的 `Datas/Network` 小网络实验室
    std::filesystem::create_directories("Datas/Network");

    // 用命令获取设备的 WiFi 接口信息，就像给 WiFi 接口做个小检查
    std::string wifiInterfaceInformation = executeCommand(
        "adb shell \"ip addr show wlan0 2>/dev/null || ip addr show wlan1\"");

    // 用命令获取设备当前的 WiFi 连接信息，看看它和哪个 WiFi 小天使牵手啦
    std::string currentWifiConnectionInformation =
        executeCommand("adb shell \"dumpsys wifi | grep 'mWifiInfo'\"");

    // 用命令获取设备所有的网络接口信息，就像把设备的网络小触角都摸一遍
    std::string allNetworkInterfacesInformation =
        executeCommand("adb shell ip addr");

    // 用命令获取设备的路由表信息，看看网络数据是怎么跑来跑去的
    std::string networkRoutingTableInformation =
        executeCommand("adb shell ip route");

    // 把这些网络信息都小心翼翼地写到一个文件里，就像写一本网络小日记
    try {
        // 获取当前的时间，给日记加上一个时间戳
        auto currentSystemClockTimePoint = std::chrono::system_clock::now();
        auto currentUnixTimestamp =
            std::chrono::system_clock::to_time_t(currentSystemClockTimePoint);

        // 打开一个文件，准备开始写日记啦
        std::ofstream networkInfoReportFile(
            "Datas/Network/network_info.log");
        // 如果文件打不开，就像日记的本子坏了，会伤心地抛出一个异常哟
        if (!networkInfoReportFile.is_open()) {
            throw std::runtime_error("Cannot create network info file");
        }

        // 给日记写上一个大大的标题
        networkInfoReportFile << "=== Network Information Report ==="
                              << std::endl;
        // 写上日记的生成时间
        networkInfoReportFile << "Generated: "
                              << std::ctime(&currentUnixTimestamp);
        // 写上设备的型号，就像给日记加上一个小标签
        networkInfoReportFile << "Device Model: " << connectedDeviceModelName
                              << std::endl << std::endl;

        // 写上 WiFi 接口的信息，如果没有找到接口，就说声小遗憾哟
        networkInfoReportFile << "=== WiFi Interface Information ==="
                              << std::endl;
        networkInfoReportFile
            << (wifiInterfaceInformation.empty() ? "No WiFi interface found"
                                                 : wifiInterfaceInformation)
            << std::endl;

        // 写上当前的 WiFi 连接信息，如果没有连接，就说声小寂寞哟
        networkInfoReportFile << "=== Current WiFi Connection ==="
                              << std::endl;
        networkInfoReportFile
            << (currentWifiConnectionInformation.empty()
                    ? "No active WiFi connection"
                    : currentWifiConnectionInformation)
            << std::endl;

        // 写上所有的网络接口信息
        networkInfoReportFile << "=== All Network Interfaces ==="
                              << std::endl;
        networkInfoReportFile << allNetworkInterfacesInformation << std::endl;

        // 写上路由表信息
        networkInfoReportFile << "=== Routing Table ===" << std::endl;
        networkInfoReportFile << networkRoutingTableInformation << std::endl;

        // 写完日记，把本子合上啦
        networkInfoReportFile.close();

        // 用一个神奇的正则表达式魔法，从 WiFi 接口信息里找出 IP 地址
        std::regex ipAddressRegexPattern(
            R"(inet\s+(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}))");
        std::smatch ipAddressRegexMatch;
        // 如果成功找到了 IP 地址，就把它写到另一个小文件里，就像给 IP
        // 地址建个小档案
        if (std::regex_search(wifiInterfaceInformation, ipAddressRegexMatch,
                              ipAddressRegexPattern)) {
            std::string extractedIpAddress = ipAddressRegexMatch[1];
            std::ofstream wirelessDebugIpFile(
                "Datas/Network/wireless_debug_ip.log");
            // 如果小档案的本子打不开，也会伤心地抛出一个异常哟
            if (!wirelessDebugIpFile.is_open()) {
                throw std::runtime_error(
                    "Cannot create wireless debug IP file");
            }
            // 给小档案写上一个标题
            wirelessDebugIpFile
                << "=== ADB Wireless Debug Information ===" << std::endl;
            // 写上小档案的生成时间
            wirelessDebugIpFile << "Generated: "
                                << std::ctime(&currentUnixTimestamp);
            // 写上 IP 地址
            wirelessDebugIpFile << "IP Address: " << extractedIpAddress
                                << std::endl;
            // 写上端口号
            wirelessDebugIpFile << "Port: 5555" << std::endl;
            // 写上连接命令
            wirelessDebugIpFile << "Connection Command: adb connect "
                                << extractedIpAddress << ":5555" << std::endl;
            // 写完小档案，把本子合上啦
            wirelessDebugIpFile.close();
        }
    } catch (const std::exception& caughtException) {
        // 如果写日记或者建小档案的时候出了问题，就用红色的字伤心地告诉我们哟
        std::cerr << MODEL_ERROR_RED_COLOR_CODE
                  << "[!] Error: " << caughtException.what()
                  << MODEL_DEFAULT_WHITE_COLOR_CODE << std::endl;
    }
}

// 哇哦~`DumpSystemInfo` 函数就像是一个超级系统小护士，它会创建一个
// `Datas/System` 小病房，然后把安卓设备的系统信息都好好地记录到病房的病历本里呢
void Model::DumpSystemInfo() {
    // 建一个温馨的 `Datas/System` 小病房
    std::filesystem::create_directories("Datas/System");
    // 用命令把设备的系统信息写到病历本 `Datas/System/system_info.log` 里哟
    system("adb shell dumpsys > Datas/System/system_info.log");
}

// 哎呀呀，`ListPackages` 函数就像是一个超级软件清单小助手，它会创建一个
// `Datas/Packages`
// 小清单本，然后把安卓设备里已安装应用的包名信息都记到清单本里呢
void Model::ListPackages() {
    // 建一个整齐的 `Datas/Packages` 小清单本
    std::filesystem::create_directories("Datas/Packages");
    // 用命令把设备里已安装应用的包名信息记到清单本
    // `Datas/Packages/installed_packages.log` 里哟
    system(
        "adb shell pm list packages > Datas/Packages/installed_packages.log");
}

// 哇塞~`ExtractWhatsApp` 函数就像是一个超级 WhatsApp 小粉丝，它会创建一个
// `Datas/WhatsApp` 小粉丝俱乐部，然后把安卓设备里的 WhatsApp
// 数据都拉到俱乐部里呢
void Model::ExtractWhatsApp() {
    // 建一个热闹的 `Datas/WhatsApp` 小粉丝俱乐部
    std::filesystem::create_directories("Datas/WhatsApp");
    // 把设备里 `/sdcard/Android/data/com.whatsapp/` 这个 WhatsApp
    // 小天地里的数据都拉到俱乐部里哟
    system("adb pull /sdcard/Android/data/com.whatsapp/ Datas/WhatsApp/");
}

// 哟哟~`ExtractBrowserData` 函数就像是一个超级浏览器小探险家，它会创建一个
// `Datas/Browser`
// 小探险基地，然后去安卓设备的浏览器里探险，把有用的数据都带回来呢
void Model::ExtractBrowserData() {
    // 建一个充满冒险气息的 `Datas/Browser` 小探险基地
    std::filesystem::create_directories("Datas/Browser");

    // 启用 Chrome 浏览器的无障碍服务，就像给探险队开了一个小绿灯
    system(
        "adb shell settings put secure enabled_accessibility_services "
        "com.android.chrome/"
        "com.android.chrome.accessibility.AccessibilityService");

    // 从设备的 logcat 里找出 HTTPS URL 信息，就像在探险中发现了神秘的宝藏线索
    system("adb logcat -d | grep -i \"https\" > Datas/Browser/https_urls.log");

    // 把 Chrome 浏览器的缓存文件拉到探险基地里，就像把宝藏的小碎片都收集起来
    system(
        "adb pull /sdcard/Android/data/com.android.chrome/cache/ "
        "Datas/Browser/chrome_cache/");
    // 把 WebView 的缓存文件也拉到探险基地里，就像收集更多的宝藏碎片
    system(
        "adb pull /data/data/com.android.webview/cache/ "
        "Datas/Browser/webview_cache/");

    // 设置一个代理，就像给探险队安排了一个小向导
    system("adb shell settings put global http_proxy 192.168.1.100:8080");

    // 把 Chrome 浏览器的历史记录文件拉到探险基地里，就像打开了一本探险日记
    system(
        "adb pull /data/data/com.android.chrome/app_chrome/Default/History "
        "Datas/Browser/");
    // 用 `sqlite3`
    // 工具从历史记录文件里找出有用的信息，就像从日记里读出精彩的故事
    system(
        "sqlite3 Datas/Browser/History \"SELECT * FROM urls;\" > "
        "Datas/Browser/browsing_history.log");
}

// 哇哦~`ExtractNotifications` 函数就像是一个超级通知小使者，它会创建一个
// `Datas/Notifications` 小通知站，然后把安卓设备里的通知信息都收集到通知站里呢
void Model::ExtractNotifications() {
    // 建一个明亮的 `Datas/Notifications` 小通知站
    std::filesystem::create_directories("Datas/Notifications");
    // 用命令把设备里的通知信息收集到通知站的小本子
    // `Datas/Notifications/notifications.log` 里哟
    system(
        "adb shell dumpsys notification > "
        "Datas/Notifications/notifications.log");
}

// 呐呐~`DumpSysFromSpecificPackage` 函数就像是一个专属小管家，
// 它会为指定的应用包运行 `adb shell dumpsys package` 命令，
// 把输出的系统信息小心翼翼地保存到小档案里哟
void Model::DumpSysFromSpecificPackage(
    const std::string &targetPackageName) {
    // 建一个专属的 `Datas/SysFromSpecificPackage` 小档案室
    std::filesystem::create_directories("Datas/SysFromSpecificPackage");

    // 把指定包的系统信息写进以包名命名的小档案里哟
    system(("adb shell dumpsys package " + targetPackageName +
            " > Datas/SysFromSpecificPackage/" + targetPackageName + ".log")
               .c_str());
}

// 呀呀~`RunApkTool` 函数就像是一个拆包小能手，
// 它会用 apktool 把 APK 文件反编译出来，然后像小侦探一样
// 在反编译出的文件里嗅探敏感关键词，把发现都记录到报告里呢
void Model::RunApkTool(const std::string &apkFilePath) {
    // 准备一个小口袋，用来装当前读到的每一行文字
    std::string currentFileLineContent;
    // 准备一个小计数器，用来记录扫描到第几行啦
    int currentLineNumber = 0;

    // 反编译输出的小目录路径，就是去掉 APK 扩展名的那个名字哟
    std::string decompiledOutputDirectoryPath =
        apkFilePath.substr(0, apkFilePath.find_last_of("."));
    // 反编译出来的 AndroidManifest.xml 小文件路径
    std::string androidManifestFilePath =
        decompiledOutputDirectoryPath + "/AndroidManifest.xml";
    // 反编译出来的 strings.xml 资源小文件路径
    std::string stringsResourceFilePath =
        decompiledOutputDirectoryPath + "/res/values/strings.xml";
    // 敏感数据扫描报告的输出小文件路径
    std::string sensitiveDataReportFilePath =
        apkFilePath + "_敏感数据泄漏.log";

    // 敏感关键词小清单，都是容易藏着秘密的小词哟
    std::vector<std::string> sensitiveKeywordList = {
        "token", "key", "firebase", "secret", "public",
        "aws",   "api", "tencent",  "auth",
    };

    std::cout << "正在解包 APK..." << std::endl;
    system(("apktool d -f " + apkFilePath + " -o " +
            decompiledOutputDirectoryPath)
               .c_str());

    std::cout << "正在扫描敏感关键词..." << std::endl;

    // 打开报告小本子，准备开始记录发现哟
    std::ofstream scanReportOutputStream(sensitiveDataReportFilePath);
    if (!scanReportOutputStream.is_open()) {
        std::cerr << "无法打开输出文件: " << sensitiveDataReportFilePath
                  << std::endl;
        return;
    }

    // 小嗅探犬技能：打开一个文件，逐行闻一闻里面有没有敏感关键词哟
    auto scanFileForSensitiveKeywords = [&](const std::string& sourceFilePathToScan) {
        std::ifstream sourceFileInputStream(sourceFilePathToScan);
        if (!sourceFileInputStream.is_open()) {
            std::cerr << "无法打开文件: " << sourceFilePathToScan
                      << std::endl;
            return;
        }

        while (std::getline(sourceFileInputStream, currentFileLineContent)) {
            ++currentLineNumber;

            for (const auto& currentSensitiveKeyword : sensitiveKeywordList) {
                if (currentFileLineContent.find(currentSensitiveKeyword) !=
                    std::string::npos) {
                    scanReportOutputStream
                        << "[文件: " << sourceFilePathToScan << "] 第 "
                        << currentLineNumber << " 行包含 ["
                        << currentSensitiveKeyword
                        << "]: " << currentFileLineContent << "\n";
                    break;
                }
            }
        }

        sourceFileInputStream.close();
    };

    // 让小嗅探犬去闻一闻 AndroidManifest.xml 和 strings.xml 哟
    scanFileForSensitiveKeywords(androidManifestFilePath);
    scanFileForSensitiveKeywords(stringsResourceFilePath);

    // 闻完啦，把报告小本子合上哟
    scanReportOutputStream.close();

    std::cout << "扫描完成，结果已写入: " << sensitiveDataReportFilePath
              << std::endl;
}

// 小嗅探犬技能~闻一闻这行文字里有没有藏着敏感小关键词哟
// 只要闻到一个，就立刻开心地摇尾巴返回 true 哒~
[[nodiscard]] bool ContainsAny(
    std::string_view fileLineContent,
    const std::vector<std::string> &sensitiveKeywordList) {
    for (const auto &currentSensitiveKeyword : sensitiveKeywordList) {
        if (fileLineContent.find(currentSensitiveKeyword) !=
            std::string_view::npos) {
            return true;
        }
    }
    return false;
}

// 呐呐~`Extract微信` 函数就像是一个微信小魔法师，
// 它会先试着从 /sdcard 提取微信数据，不行的话就戴上 root 小皇冠
// 从 /data/data 里面把数据偷偷搬出来哟
void Model::Extract微信() {
    // 微信数据的小仓库路径哒
    const std::string weChatExtractionOutputDirectory = "Datas/微信";
    std::filesystem::create_directories(weChatExtractionOutputDirectory);

    std::cout << "检查设备连接状态..." << std::endl;
    // 小医生技能：不同系统要用不同的小黑洞来吞掉输出哟~
#ifdef _WIN32
    if (system("adb get-state > nul 2>&1") != 0) {
#else
    if (system("adb get-state > /dev/null 2>&1") != 0) {
#endif
        std::cerr << "设备未连接。" << std::endl;
        return;
    }

    std::cout << "正在尝试提取 /sdcard 路径..." << std::endl;
    // 先试着从 /sdcard 里的微信小天地把数据拉出来哟
    std::string sdcardPullCommand =
        "adb pull /sdcard/Android/data/com.tencent.mm/ \"" +
        weChatExtractionOutputDirectory + "/sdcard/\"";
    int sdcardPullExitCode = system(sdcardPullCommand.c_str());

    if (sdcardPullExitCode == 0) {
        std::cout << "成功提取 /sdcard 目录。" << std::endl;
        return;
    }

    std::cout << "/sdcard 不可访问，尝试 /data/data 路径（需要 root）..."
              << std::endl;
    // /sdcard 进不去啦，就戴上 root 小皇冠，把 /data/data 里的微信数据
    // 先复制到 sdcard 的临时小房间，再拉出来哟
    std::string rootCopyToSdcardCommand =
        "adb shell su -c 'cp -r /data/data/com.tencent.mm /sdcard/wechat_tmp "
        "&& chmod -R 777 /sdcard/wechat_tmp'";
    int rootCopyExitCode = system(rootCopyToSdcardCommand.c_str());

    if (rootCopyExitCode != 0) {
        std::cerr << "无法访问 /data/data。需要 root 权限。" << std::endl;
        return;
    }

    // 把临时小房间里的微信数据拉到我们的小仓库里哟
    std::string temporaryDirectoryPullCommand =
        "adb pull /sdcard/wechat_tmp \"" + weChatExtractionOutputDirectory +
        "/data_data/\"";
    if (system(temporaryDirectoryPullCommand.c_str()) == 0) {
        std::cout << "成功提取 /data/data 目录。" << std::endl;
    } else {
        std::cerr << "复制 /data/data 失败。" << std::endl;
    }
}
