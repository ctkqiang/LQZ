#include "controller.h"

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <thread>

void Controller::Run() {
    view.ClearScreen();
    view.DisplayHeader();

    try {
        model.CheckAdb();
    } catch (const std::exception& e) {
        view.ShowError("ADB安装检查失败");
        return;
    }

    bool deviceConnected = model.CheckDevice();
    view.ShowDeviceStatus(deviceConnected, model.GetDeviceModel());
    view.DisplayHelp();

    if (!deviceConnected) {
        view.ShowError("未检测到手机设备");
        return;
    }

    std::string command;
    while (true) {
        view.GetInput("\n命令> ", command);

        try {
            if (command == "exit" || command == "0") {
                view.ShowMessage("正在退出程序...", view.YELLOW);
                break;
            }

            if (command == "clear") {
                view.ClearScreen();
                continue;
            }

            if (command == "about") {
                view.DisplayHeader();
                continue;
            }

            if (command == "help" || command == "?") {
                view.DisplayHelp();
                continue;
            }

            if (command == "1") {
                view.ShowMessage("正在获取通话记录...", view.BLUE);
                model.DumpCallLogs();
                view.ShowSuccess("通话记录已导出");
            } else if (command == "2") {
                view.ShowMessage("正在导出短信...", view.BLUE);
                model.DumpSMS();
                view.ShowSuccess("短信已导出");
            } else if (command == "3") {
                view.ShowMessage("正在导出照片...", view.BLUE);
                model.DumpPhotos();
                view.ShowSuccess("照片已导出");
            } else if (command == "4") {
                view.ShowMessage("正在导出视频...", view.BLUE);
                model.DumpVideos();
                view.ShowSuccess("视频已导出");
            } else if (command == "5") {
                view.ShowMessage("正在导出应用...", view.BLUE);
                model.DumpApks();
                view.ShowSuccess("应用已导出");
            } else if (command == "6") {
                view.ShowMessage("正在导出文档...", view.BLUE);
                model.DumpDocuments();
                view.ShowSuccess("文档已导出");
            } else if (command == "7") {
                view.ShowMessage("正在提取敏感信息...", view.BLUE);
                model.ExtractSensitiveInfo();
                view.ShowSuccess("信息提取完成");
            } else if (command == "8") {
                model.ConnectWirelessly();
            } else if (command == "9" || command == "wifi") {
                view.ShowMessage("正在获取网络信息...", view.BLUE);
                model.GetWifiAddress();
                view.ShowSuccess("网络信息已导出");
            } else if (command == "10" || command == "sysinfo") {
                view.ShowMessage("正在导出系统信息...", view.BLUE);
                model.DumpSystemInfo();
                view.ShowSuccess("系统信息已导出");
            } else if (command == "11" || command == "packages") {
                view.ShowMessage("正在获取应用列表...", view.BLUE);
                model.ListPackages();
                view.ShowSuccess("应用列表已导出");
            } else if (command == "12" || command == "whatsapp") {
                view.ShowMessage("正在提取WhatsApp数据...", view.BLUE);
                model.ExtractWhatsApp();
                view.ShowSuccess("WhatsApp数据已导出");
            } else if (command == "13" || command == "browser") {
                view.ShowMessage("正在提取浏览器数据...", view.BLUE);
                model.ExtractBrowserData();
                view.ShowSuccess("浏览器数据已导出");
            } else if (command == "14" || command == "notifications") {
                view.ShowMessage("正在提取通知记录...", view.BLUE);
                model.ExtractNotifications();
                view.ShowSuccess("通知记录已导出");
            } else {
                view.ShowError("未知命令，输入 'help' 或 '?' 查看可用命令");
            }

            if (!model.CheckDevice()) {
                view.ShowError("设备连接已断开");
                break;
            }

        } catch (const std::exception& e) {
            view.ShowError(std::string("执行命令时出错: ") + e.what());
        }
    }
}
