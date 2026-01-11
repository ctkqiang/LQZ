#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <regex>
#include <string>
// #include <gtkmm/application.h>

#include "controller/controller.h"
// #include "view/gui.h"

int main(int argc, char* argv[]) {
    Controller App;
    App.Run();

    // auto app = Gtk::Application::create("xin.ctkqiang.lqz");

    // return app->make_window_and_run<GUI>(argc, argv);

    return 0x0;
}