#pragma once

#ifndef GUI_H
#define GUI_H

// 哇哦~这里是超可爱的图形界面小舞台呀
// GUI 小仙子会继承 Gtk::Window，为大家带来漂亮的图形界面哟~
// （1.1 版本即将登场，敬请期待哒~）

#include <gtkmm.h>

// 呀~这个 GUI 类就像是一个超级梦幻的小窗口哟
// 它会用 GTKmm 魔法把可爱的按钮和界面展示给用户呢~
class GUI : public Gtk::Window {
   public:
    // 小窗口的魔法诞生仪式~
    GUI();
    // 小窗口的温柔告别仪式~
    ~GUI() override;

   protected:
    // 按钮被点击时的小魔法回调哟
    virtual void onGreetingButtonClicked();

    // 小秘密：这里藏着会打招呼的小按钮哒
    Gtk::Button m_greetingButton;
};

#endif
