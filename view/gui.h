#pragma once

#ifndef GUI_H
#define GUI_H

class GUI : public Gtk::Window {
public:
    GUI();
    ~GUI() override;

protected:
    virtual void on_button_clicked();

    Gtk::Button m_button;
};

#endif 
