// #include <gtkmm/application.h>

#include "controller/controller.h"
// #include "view/gui.h"

int main(int argc, char* argv[]) {
    // 小管家登场~创建控制器并开启整个程序的魔法流程哟
    Controller phoneForensicController;
    phoneForensicController.Run();

    // auto app = Gtk::Application::create("xin.ctkqiang.lqz");

    // return app->make_window_and_run<GUI>(argc, argv);

    // 在1.1版本中实现此功能

    return 0x0;
}
