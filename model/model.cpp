#include "model.h"

#include <cstdio>
#include <cstdlib>

const std::string ADB_CONNECT = "adb connect <device_ip>:5555";
const std::string ADB_DISCONNECT = "adb disconnect <device_ip>:5555";
const std::string ADB_PULL = "adb pull <remote_path> <local_path>";
const std::string ADB_SHELL = "adb shell ";
const char* ADB_BACKUP_SMS =
    "adb backup -noapk -f Datas/Phone_dump_sms.ab -nocompress "
    "com.android.providers.telephony";

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