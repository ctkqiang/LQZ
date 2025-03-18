#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../model/model.h"
#include "../view/view.h"

// 哇哦~这里是控制器的魔法指挥中心呀 (๑•. •๑)
// 所有可爱的操作指令都在这里汇聚，就像小仙子们在跳圆圈舞呢~

// 呀~这个 Controller 类就像是一个超级魔法管家呢 (✿◠‿◠)
// 它会负责协调 Model 和 View 的互动，就像贴心的小女仆一样~
class Controller {
    // 小秘密：这里藏着数据模型小管家 Model 哟 (๑•ω•๑)
    Model model;
    // 小秘密：这里藏着视图小助手 View 哟 (๑•ω•๑)
    View view;

   public:
    // 小管家的魔法启动仪式来啦~
    // 它会开启整个程序的魔法流程，让 Model 和 View 一起翩翩起舞哟
    void Run();

    // 小管家的菜单魔法在此~
    // 它会展示一个美美的功能菜单，就像递给用户一本魔法说明书哟
    void DisplayMenu();
};

#endif