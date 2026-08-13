#include "controller/controller.h"

int main(int argc, char* argv[]) {
    // 无参数启动:进入交互式 TUI;带参数:CLI 模式(子命令/--json 等)
    if (argc == 1) {
        // 小管家登场~创建控制器并开启整个程序的魔法流程哟
        Controller phoneForensicController;
        return phoneForensicController.Run();
    }
    return Controller::RunCli(argc, argv);
}
