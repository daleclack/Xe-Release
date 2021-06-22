#pragma once

#include <gtkmm.h>

class MyWin : public Gtk::ApplicationWindow{
public:
    MyWin();
private:
    //Child Widgets
    Glib::RefPtr<Gtk::Builder> menu_builder;

    //Main window
    Gtk::Image background;
    Gtk::Overlay overlay;
    Gtk::Box btn_box;
    Gtk::ComboBoxText combo;
    Gtk::Button btn_ver;

    //TitleBar and menu
    Gtk::HeaderBar header;
    Gtk::MenuButton menubtn;
    Gtk::PopoverMenu popover;
    void titlebar_init();

    //Backgrounds
    void background1();
    void background2();
};