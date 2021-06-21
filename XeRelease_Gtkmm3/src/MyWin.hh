#pragma once

#include <gtkmm.h>

class MyWin : public Gtk::Window{
public:
    MyWin();
private:
    //Child Widgets
    Gtk::Image background;
    Gtk::Overlay overlay;
    Gtk::Box btn_box;
    Gtk::ComboBoxText combo;
    Gtk::Button btn_ver;
    Gtk::HeaderBar header;
};