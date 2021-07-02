#pragma once

#include <gtkmm.h>
#include "MyDialog.hh"

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
    Gtk::Label api_label;
    Gtk::ComboBoxText combo;
    Gtk::Button btn_ver;

    //TitleBar and menu
    Gtk::HeaderBar header;
    Gtk::MenuButton menubtn;
    Gtk::PopoverMenu popover;
    void titlebar_init();

    //Dialogs
    MsgBox msg_dialog;
    MyDialog input_dialog;

    //Backgrounds
    void background1();
    void background2();

    //Version Configs
    FILE *fp;//file pointer to read file
    struct tm *local;
    char api_version[57];
    void config_lts();
    void config_stable();
    void config_devel();
    bool get_config(const char *filename);

    //Signal Handlers
    void about_dialog();
    void main_releases();
};