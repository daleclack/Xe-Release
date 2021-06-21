#pragma once

#include <gtkmm.h>

class MyWin : public Gtk::Window{
public:
    MyWin();
private:
    Gtk::Image background;
    Gtk::Overlay overlay;
};