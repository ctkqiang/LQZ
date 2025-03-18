#ifndef MODEL_H
#define MODEL_H

#include <filesystem>
#include <regex>
#include <string>

// 哇哦~这里是数据模型类的小窝窝呀 (๑•. •๑) 所有可爱的数据操作都在这里诞生哟~
// 小仙子们正在这里翩翩起舞，守护着每一个重要的字节呢~

// 把文件系统命名空间简化成可爱的 fs 小昵称啦
namespace fs = std::filesystem;

// 呀~这个 Model 类就像是一个超级魔法管家呢 (✿◠‿◠)
// 它能帮我们处理各种安卓设备的数据操作，就像贴心的小女仆一样~
class Model {
    // 小秘密：这里藏着设备型号信息哟 (๑•ω•๑)
    std::string deviceModel;

    // 小魔法师 executeCommand 在这里呢~
    // 它能执行各种系统命令，还会把结果像小礼物一样包装好带回来哟
    std::string executeCommand(const std::string& cmd);

   public:
    // 小门卫 CheckDevice 来啦~
    // 它会认真检查有没有连接安卓设备，如果有的话还会悄悄记住设备型号呢
    bool CheckDevice();

    // 小医生 CheckAdb 在此~
    // 它会仔细检查电脑里有没有安装 ADB 工具，如果没有的话还会帮忙安装哟
    void CheckAdb();

    // 小管家 DumpSMS 出动~
    // 它会把安卓设备里的短信都好好备份到小房间 Datas 里呢
    void DumpSMS();

    // 小天使 DumpPhotos 来啦~
    // 它会把设备里的照片都收集到 Datas 小相册里，就像收集星星一样哟
    void DumpPhotos();

    // 小快递员 DumpDownloads 工作中~
    // 它会把设备里下载的文件都送到 Datas 小包裹站，一个都不会漏掉哒
    void DumpDownloads();

    // 小秘书 DumpContacts 在此~
    // 它会把联系人信息和通话记录都整理到 Datas 小仓库里，超有条理哒
    void DumpContacts();

    // 小邮差 DumpMessage 出发~
    // 它会把短信数据库文件都安全地送到 Datas 小信箱里哟
    void DumpMessage();

    // 小搬运工 Dumpfiles 上线~
    // 它会把整个 SD 卡的内容都搬到 Datas/sdcard 大仓库里呢
    void Dumpfiles();

    // 小使者 ConnectWirelessly 工作啦~
    // 它会让我们输入设备 IP 地址，然后用无线魔法和设备牵起手手哟
    void ConnectWirelessly();

    // 小秘书 Shell 在此待命~
    // 我们把要执行的命令告诉它，它就会乖乖去执行并把结果报告给我们哟
    void Shell(std::string cmd);

    // 小侦探 GetDeviceModel 来啦~
    // 调用它就能知道连接的设备是什么型号，超厉害哒
    std::string GetDeviceModel() const;

    // 小记录员 DumpCallLogs 工作中~
    // 它会把通话记录都记到 Datas/CallLogs 小本子里，超详细哟
    void DumpCallLogs();

    // 小影迷 DumpVideos 在此~
    // 它会把设备里的视频都收集到 Datas 小电影院里，想看就看哟
    void DumpVideos();

    // 小收藏家 DumpApks 出动~
    // 它会把 APK 文件信息和文件都收集到 Datas/APKs 小软件库里呢
    void DumpApks();

    // 小管理员 DumpDocuments 工作啦~
    // 它会把文档文件都整理到 Datas/Documents 小文档室里，超整齐哒
    void DumpDocuments();

    // 小侦探 ExtractSensitiveInfo 出动~
    // 它会把敏感信息都收集到 Datas/Sensitive 小秘密基地里哟
    void ExtractSensitiveInfo();

    // 小专家 GetWifiAddress 在此~
    // 它会把 WiFi 地址和网络信息都研究清楚，记到 Datas/Network 小实验室里哟
    void GetWifiAddress();

    // 小护士 DumpSystemInfo 工作中~
    // 它会把设备的系统信息都记录到 Datas/System 小病房里呢
    void DumpSystemInfo();

    // 小助手 ListPackages 在此~
    // 它会把已安装应用的包名信息都列到 Datas/Packages 小清单本里哟
    void ListPackages();

    // 小粉丝 ExtractWhatsApp 出动~
    // 它会把 WhatsApp 数据都收集到 Datas/WhatsApp 小粉丝俱乐部里呢
    void ExtractWhatsApp();

    // 小探险家 ExtractBrowserData 工作啦~
    // 它会去浏览器里探险，把有用的数据都带回来放到 Datas/Browser 小基地里哟
    void ExtractBrowserData();

    // 小使者 ExtractNotifications 在此~
    // 它会把通知信息都收集到 Datas/Notifications 小通知站里哟
    void ExtractNotifications();
};

#endif