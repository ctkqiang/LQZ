#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../model/model.h"
#include "../view/view.h"

class Controller {
    Model model;
    View view;

   public:
    void Run();
    void DisplayMenu();
};

#endif