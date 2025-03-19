#include "controller.h"

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <thread>

// 哇哦~这里是控制器的魔法指挥中心呀 (๑•. •๑)
// 所有可爱的操作指令都在这里汇聚，就像小仙子们在跳圆圈舞呢~

// 小管家 Controller 来啦~它会负责协调 Model 和 View 的互动哟
void Controller::Run() {
    // 让 View 小助手先把屏幕擦得亮晶晶的~
    view.ClearScreen();
    // 展示一个美美的欢迎标题页，就像拉开魔法舞台的帷幕~
    view.DisplayHeader();

    try {
        // 让 Model 小医生去检查 ADB 工具有没有安装好哟~
        model.CheckAdb();
    } catch (const std::exception& e) {
        // 如果检查失败，View 小助手会用红色字体伤心地报告错误
        view.ShowError("ADB安装检查失败");
        return;
    }

    // 让 Model 小门卫检查一下有没有连接安卓设备呢~
    bool deviceConnected = model.CheckDevice();
    // View 小助手会用可爱的图标显示设备连接状态和型号哟
    view.ShowDeviceStatus(deviceConnected, model.GetDeviceModel());
    // 展示一个小帮助菜单，就像递给用户一本魔法说明书~
    view.DisplayHelp();

    // 如果没有检测到设备，View 小助手会用红色字体提醒我们哟
    if (!deviceConnected) {
        view.ShowError("未检测到手机设备");
        return;
    }

    // 准备一个小本子，用来记录用户输入的命令哒
    std::string command;
    // 进入一个魔法循环，直到用户说退出哟
    while (true) {
        // View 小助手会用黄色字体提示用户输入命令哒
        view.GetInput("\n命令> ", command);

        try {
            // 如果输入的是 exit 或 0，就像小仙子要回家啦~
            if (command == "exit" || command == "0") {
                view.ShowMessage("正在退出程序...", view.YELLOW);
                break;
            }

            // 如果输入 clear，View 小助手会把屏幕擦得像新的一样哟
            if (command == "clear") {
                view.ClearScreen();
                continue;
            }

            // 如果输入 about，View 小助手会重新展示欢迎标题页哒
            if (command == "about") {
                view.DisplayHeader();
                continue;
            }

            // 如果输入 help 或 ?，View 小助手会再次展示帮助菜单哟
            if (command == "help" || command == "?") {
                view.DisplayHelp();
                continue;
            }

            // 命令 1：让 Model 小记录员去获取通话记录啦~
            if (command == "1") {
                view.ShowMessage("正在获取通话记录...", view.BLUE);
                model.DumpCallLogs();
                view.ShowSuccess("通话记录已导出");
            }
            // 命令 2：让 Model 小管家去备份短信哟~
            else if (command == "2") {
                view.ShowMessage("正在导出短信...", view.BLUE);
                model.DumpSMS();
                view.ShowSuccess("短信已导出");
            }
            // 命令 3：让 Model 小天使去收集照片啦~
            else if (command == "3") {
                view.ShowMessage("正在导出照片...", view.BLUE);
                model.DumpPhotos();
                view.ShowSuccess("照片已导出");
            }
            // 命令 4：让 Model 小影迷去收集视频哟~
            else if (command == "4") {
                view.ShowMessage("正在导出视频...", view.BLUE);
                model.DumpVideos();
                view.ShowSuccess("视频已导出");
            }
            // 命令 5：让 Model 小收藏家去收集应用 APK 哟~
            else if (command == "5") {
                view.ShowMessage("正在导出应用...", view.BLUE);
                model.DumpApks();
                view.ShowSuccess("应用已导出");
            }
            // 命令 6：让 Model 小管理员去整理文档哒~
            else if (command == "6") {
                view.ShowMessage("正在导出文档...", view.BLUE);
                model.DumpDocuments();
                view.ShowSuccess("文档已导出");
            }
            // 命令 7：让 Model 小侦探去提取敏感信息哟~
            else if (command == "7") {
                view.ShowMessage("正在提取敏感信息...", view.BLUE);
                model.ExtractSensitiveInfo();
                view.ShowSuccess("信息提取完成");
            }
            // 命令 8：让 Model 小使者去无线连接设备哒~
            else if (command == "8") {
                model.ConnectWirelessly();
            }
            // 命令 9/wifi：让 Model 小专家去获取网络信息哟~
            else if (command == "9" || command == "wifi") {
                view.ShowMessage("正在获取网络信息...", view.BLUE);
                model.GetWifiAddress();
                view.ShowSuccess("网络信息已导出");
            }
            // 命令 10/sysinfo：让 Model 小护士去获取系统信息哒~
            else if (command == "10" || command == "sysinfo") {
                view.ShowMessage("正在导出系统信息...", view.BLUE);
                model.DumpSystemInfo();
                view.ShowSuccess("系统信息已导出");
            }
            // 命令 11/packages：让 Model 小助手去获取应用列表哟~
            else if (command == "11" || command == "packages") {
                view.ShowMessage("正在获取应用列表...", view.BLUE);
                model.ListPackages();
                view.ShowSuccess("应用列表已导出");
            }
            // 命令 12/whatsapp：让 Model 小粉丝去提取 WhatsApp 数据哒~
            else if (command == "12" || command == "whatsapp") {
                view.ShowMessage("正在提取WhatsApp数据...", view.BLUE);
                model.ExtractWhatsApp();
                view.ShowSuccess("WhatsApp数据已导出");
            }
            // 命令 13/browser：让 Model 小探险家去提取浏览器数据哟~
            else if (command == "13" || command == "browser") {
                view.ShowMessage("正在提取浏览器数据...", view.BLUE);
                model.ExtractBrowserData();
                view.ShowSuccess("浏览器数据已导出");
            }
            // 命令 14/notifications：让 Model 小使者去收集通知记录哒~
            else if (command == "14" || command == "notifications") {
                view.ShowMessage("正在提取通知记录...", view.BLUE);
                model.ExtractNotifications();
                view.ShowSuccess("通知记录已导出");
            }

            else if (command == "15" || command == "dumpsys") {
            }

            // 如果输入了不认识的命令，View 小助手会用红色字体提醒哟
            else {
                view.ShowError("未知命令，输入 'help' 或 '?' 查看可用命令");
            }

            // 检查一下设备连接状态，如果断开就结束魔法哟
            if (!model.CheckDevice()) {
                view.ShowError("设备连接已断开");
                break;
            }

        } catch (const std::exception& e) {
            // 如果执行命令时出错，View 小助手会用红色字体报告错误信息哒
            view.ShowError(std::string("执行命令时出错: ") + e.what());
        }
    }
}