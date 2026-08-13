#include "gui.h"

#include <iostream>

// GUI 小仙子登场啦~窗口标题是 "LQZ"，按钮上写着可爱的 "Click me!" 哟
GUI::GUI()
    : Gtk::Window("LQZ"),
      m_greetingButton("Click me!") {
    // 在1.1版本中实现此功能：把按钮加进窗口并连上点击信号哟~
}
