#include "controller.h"

#include <cstdlib>
#include <iostream>
#include <memory>

void Controller::Run() {
    view.ClearScreen();
    view.DisplayHeader();

    if (!model.CheckDevice()) {
        view.ShowError("未检测到手机设备");
        exit(1);
    }
}