#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <cstddef>
#include <string>

#include "../model/model.h"
#include "../view/view.h"

// 哇哦~这里是控制器的魔法指挥中心呀
// 所有可爱的操作指令都在这里汇聚，就像小仙子们在跳圆圈舞呢~

// 呀~这个 Controller 类就像是一个超级魔法管家呢
// 它会负责协调 Model 和 View 的互动，就像贴心的小女仆一样~
// 现在它还会说两种语言:交互式 TUI 与 CLI 子命令，共用同一份操作注册表哟~
class Controller {
    // 小秘密：这里藏着数据模型小管家 Model 哟
    Model forensicDataModel;
    // 小秘密：这里藏着视图小助手 View 哟
    View consoleUserInterface;

    // CLI 模式的待连接 IP(connect 子命令用)
    std::string pendingConnectIp_;
    // 最近一次操作的证据信息(--json 输出用)
    std::string lastManifestSha256_;
    std::size_t lastArtifactCount_ = 0;
    std::string lastOutputRoot_;
    std::string lastCliError_;

   public:
    // 小管家的魔法启动仪式来啦~(交互式,无参数启动时调用)
    // 返回进程退出码:0 正常,1 ADB 检查失败
    int Run();

    // CLI 入口:解析参数、分发子命令、输出 --json 结果
    static int RunCli(int argc, char* argv[]);

   private:
    // RunCli 的实例实现(需要访问 Model/View 成员)
    int runCliImpl(int argc, char* argv[]);
    // 交互与 CLI 共享的执行体:menuId 找不到返回 -1,执行失败返回 1,成功 0
    int runOperation(const std::string& menuId, bool jsonMode);
    // --json 结果输出(stdout 唯一内容)
    void emitJsonResult(const std::string& command, int exitCode,
                        bool withArtifacts);
};

#endif
