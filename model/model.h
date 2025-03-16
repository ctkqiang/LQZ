#ifndef MODEL_H
#define MODEL_H

#include <filesystem>
#include <regex>
#include <string>

namespace fs = std::filesystem;

class Model {
    std::string deviceModel;

    std::string executeCommand(const std::string& cmd);

   public:
    bool CheckDevice();
    void CheckAdb();
    void DumpSMS();
    void DumpPhotos();
    void DumpDownloads();
    void DumpContacts();
    void DumpMessage();
    void Dumpfiles();

    void ConnectWirelessly();

    void Shell(std::string cmd);
    std::string GetDeviceModel() const;

    void DumpCallLogs();
    void DumpVideos();
    void DumpApks();
    void DumpDocuments();
    void ExtractSensitiveInfo();

    void GetWifiAddress();
    void DumpSystemInfo();
    void ListPackages();
    void ExtractWhatsApp();
    void ExtractBrowserData();
    void ExtractNotifications();
};

#endif