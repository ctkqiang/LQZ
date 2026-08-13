#ifndef VIEW_H
#define VIEW_H

#include <string>

// 视图层:负责终端界面(TUI)的渲染与交互提示
// 设计语言:粉色标题与面板边框 + 青色线条的取证终端外壳

// View 类负责和用户互动,用颜色区分信息等级
class View {
   public:
    // 警示红:错误信息跳出来啦
    inline static const std::string RED{"\033[91m"};
    // 天空蓝:操作提示的清爽颜色
    inline static const std::string BLUE{"\033[94m"};
    // 终端绿:成功信息
    inline static const std::string GREEN{"\033[92m"};
    // 琥珀黄:警告提示
    inline static const std::string YELLOW{"\033[93m"};
    // 复位:回到默认颜色
    inline static const std::string WHITE{"\033[0m"};
    // 冰青:线条与内容边框
    inline static const std::string CYAN{"\033[96m"};
    // 软粉:品牌主色,标题与面板边框
    inline static const std::string PINK{"\033[95m"};
    // 暗灰:次要的说明信息
    inline static const std::string DIM{"\033[2m"};
    // 加粗:强调标题
    inline static const std::string BOLD{"\033[1m"};

   public:
    // 启动横幅:ASCII 手机图形 + 取证终端外壳信息面板
    void DisplayHeader();

    // 模块菜单:数据提取、系统信息与常用命令
    void DisplayHelp();

    // 设备连接状态面板
    // isDeviceConnected 是是否连接的小开关,deviceModelName 是设备型号小标签
    void ShowDeviceStatus(bool isDeviceConnected,
                          const std::string& deviceModelName = "");

    // 任务进度条
    // currentOperationName 是当前操作的小名称,completionPercentage 是完成度小数值
    void ShowProgress(const std::string& currentOperationName,
                      int completionPercentage);

    // 错误提示,红色 [err] 标签
    void ShowError(const std::string& displayMessage);

    // 成功提示,绿色 [ ok ] 标签
    void ShowSuccess(const std::string& displayMessage);

    // 按指定颜色打印 [ * ] 标签的普通提示
    void ShowMessage(const std::string& displayMessage,
                     const std::string& colorCode);

    // 获取用户输入
    void GetInput(const std::string& promptMessage, std::string& userInputBuffer);

    // 清空屏幕
    void ClearScreen();

    // CLI 模式开关:true 时面板与横幅静默,
    // 提示信息改走 stderr 且不带 ANSI 颜色(保证 --json 的 stdout 纯净)
    void SetCliMode(bool cliMode);

   private:
    // 获取当前操作系统类型
    // 会返回 "Windows"、"MacOS" 或 "Linux" 等小标签
    [[nodiscard]] std::string GetOSType();

    // CLI 模式标记
    bool cliMode_ = false;
};

#endif