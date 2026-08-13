#ifndef MODEL_H
#define MODEL_H

#include <string>

// 哇哦~欢迎来到超可爱的数据模型类小窝窝呀
// 这里就是 Model 小管家的大本营，负责和安卓设备说悄悄话，
// 把设备里的宝贝数据都小心翼翼地搬回我们的电脑上哟~

class Model {
    // 小秘密：这里藏着已连接设备的型号小标签哒
    // 由 CheckDevice 小门卫在探测设备成功后偷偷填好哟~
    std::string connectedDeviceModelName;

    // 小魔法师技能：执行一条系统命令，并把命令输出的内容都收集起来哟~
    // 如果命令执行失败了，就会返回一个空空的字符串哒
    std::string executeCommand(const std::string &shellCommandToExecute);

    // 小搬运工技能：把设备上的远程文件拉到电脑本地的小房间里哟~
    // 如果本地小房间还不存在，会先把它建好哒
    void adbPull(const std::string &remoteDeviceFilePath,
                 const std::string &localDestinationDirectoryPath);

   public:
    // 小侦探技能：返回缓存的设备型号小标签~
    // 这个小标签由 CheckDevice() 在探测设备成功后偷偷填好哟
    [[nodiscard]] std::string GetDeviceModel() const;

    // 小医生技能：检查 ADB 工具有没有安装好啦~
    // 如果没安装，会尝试在 macOS/Windows 上自动安装哟；
    // 实在找不到 ADB 的话，会伤心地退出程序呢
    void CheckAdb();

    // 小门卫技能：用 `adb devices -l` 看看设备连上没有~
    // 连上的话会把设备型号偷偷记下来，并开开心心地返回 true 哟
    [[nodiscard]] bool CheckDevice();

    // 小秘书技能：执行一条任意的 ADB shell 命令，并把结果打印出来哒~
    void Shell(const std::string &shellCommandToExecute);

    // 小使者技能：让用户输入设备 IP 地址，用无线方式和设备牵起手来哟~
    void ConnectWirelessly();

    // --- 数据提取小分队 ---

    // 小管家技能：用 `adb backup` 备份短信，再用 mvt-android 检查备份哟~
    void DumpSMS();

    // 小天使技能：把 /sdcard/Pictures 里的照片拉到 Datas/Photos/ 小相册里哒~
    void DumpPhotos();

    // 小快递员技能：把 /sdcard/Download 里的下载文件送到 Datas/Downloads/ 哟~
    void DumpDownloads();

    // 小记忆能手技能：把 contacts2.db 和 calllog.db 拉到 Datas/Contacts/ 里哒~
    void DumpContacts();

    // 小邮差技能：把 mmssms.db 放进 Datas/Messages/ 小信箱里哟~
    void DumpMessage();

    // 大搬运工技能：把整个 /sdcard/ 都搬到 Datas/sdcard/ 大仓库里呢~
    void Dumpfiles();

    // 小记录员技能：通过 content provider 查询通话记录，写进 Datas/CallLogs/ 哟~
    void DumpCallLogs();

    // 小电影迷技能：把 /sdcard/DCIM/Camera 和 /sdcard/Movies 搬到 Datas/Videos/ 哒~
    void DumpVideos();

    // 小收藏家技能：列出已安装应用，并把 /data/app/ 拉到 Datas/APKs/ 里哟~
    void DumpApks();

    // 文档管理员技能：把 /sdcard/Documents 和 /sdcard/Download 整理到 Datas/Documents/ 哟~
    void DumpDocuments();

    // 小侦探技能：把整个 /sdcard/ 搬到 Datas/Sensitive/ 提取敏感信息哟~
    void ExtractSensitiveInfo();

    // 网络专家技能：收集 WiFi 接口、路由和连接信息到 Datas/Network/ 哟~
    void GetWifiAddress();

    // 系统小护士技能：把系统属性和构建信息记录到 Datas/System/ 小病房里哟~
    void DumpSystemInfo();

    // 清单小助手技能：把所有已安装包名列到 Datas/Packages/ 清单本里哟~
    void ListPackages();

    // WhatsApp 小粉丝技能：把 WhatsApp 数据库和媒体拉到 Datas/WhatsApp/ 哟~
    void ExtractWhatsApp();

    // 浏览器探险家技能：把浏览器历史数据库拉到 Datas/Browser/ 里哟~
    void ExtractBrowserData();

    // 通知小使者技能：把通知历史记录到 Datas/Notifications/ 里哟~
    void ExtractNotifications();

    // 微信小天使技能：把微信数据库和媒体拉到 Datas/WeChat/ 里哟~
    void ExtractWeChat();

    // 专属管家技能：运行 `adb shell dumpsys <package>` 并写进 Datas/Packages/ 哟~
    void DumpSysFromSpecificPackage(const std::string &targetPackageName);

    // 小管理员技能：以指定包用户的身份运行当前进程（需要 root 权限哟）~
    void RunPackageAs();

    // 拆包小能手技能：用 apktool 把 APK 文件反编译出来哟~
    void RunApkTool(const std::string &apkFilePath);

    // 微信提取小魔法师：把微信数据从 /sdcard 或 /data/data 搬出来哟
    void Extract微信();
};

#endif
