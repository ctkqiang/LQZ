#ifndef VIEW_H
#define VIEW_H

#include <string>

// 哇哦~这里是视图小助手的魔法舞台呀 (๑•. •๑)
// 所有可爱的用户交互都在这里绽放光芒，就像小仙子在舞台上跳舞呢~

// 呀~这个 View 类就像是一个超级贴心的小司仪哟 (✿◠‿◠)
// 它会负责和用户互动，用各种可爱的提示和反馈让操作变得超有趣哒~
class View {
   public:
    // 小草莓红：用来标记重要错误信息的颜色哟
    static const std::string RED;
    // 天空蓝：用来显示操作提示的清爽颜色哒
    static const std::string BLUE;
    // 薄荷绿：成功时闪烁的治愈系颜色呀
    static const std::string GREEN;
    // 柠檬黄：温馨提示的可爱颜色哟
    static const std::string YELLOW;
    // 云朵白：默认的干净纯洁颜色哒
    static const std::string WHITE;
    // 水晶蓝：系统信息展示的高级感颜色呀
    static const std::string CYAN;

   public:
    // 小司仪登场~展示欢迎标题页的魔法仪式哟
    // 会用闪烁的星星特效和可爱 ASCII 艺术欢迎用户哒
    void DisplayHeader();

    // 小百科全书在此~展示所有可用命令的魔法手册哟
    // 会用彩虹色文字列出各种操作指令，超详细哒
    void DisplayHelp();

    // 小雷达启动~显示设备连接状态的魔法看板哟
    // connected 参数是是否连接的小开关，model 是设备型号小标签
    void ShowDeviceStatus(bool connected, const std::string& model = "");

    // 进度条小管家~显示操作进度的魔法仪表盘哟
    // operation 是当前操作的小名称，percentage 是完成度小数值
    void ShowProgress(const std::string& operation, int percentage);

    // 错误警报器~用红色闪烁文字报告错误的魔法喇叭哟
    // message 是要显示的错误小提示，会用可爱的语气读出来哒
    void ShowError(const std::string& message);

    // 成功小烟花~用绿色星星特效庆祝成功的魔法礼炮哟
    // message 是成功后的小庆祝语，会有 sparkles 音效哒
    void ShowSuccess(const std::string& message);

    // 彩色话痨模式~用指定颜色显示自定义信息的魔法泡泡哟
    // message 是要说的话，color 是选择的颜色魔法标记
    void ShowMessage(const std::string& message, const std::string& color);

    // 小话筒启动~获取用户输入的魔法吸管哟
    // prompt 是提示语小泡泡，input 是收集输入的小盒子
    void GetInput(const std::string& prompt, std::string& input);

    // 橡皮擦魔法~清空屏幕的神奇咒语哟
    // 会把屏幕擦得像新纸一样干净，还会有 "poof" 音效哒
    void ClearScreen();

   private:
    // 小侦探技能~获取当前操作系统类型的魔法扫描仪哟
    // 会返回 "Windows"、"Mac" 或 "Linux" 等小标签
    std::string GetOSType();
};

#endif