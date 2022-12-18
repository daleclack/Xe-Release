#pragma once

#include <gtkmm.h>
#include "MyPrefs.hh"

class MyWin : public Gtk::ApplicationWindow
{
public:
    MyWin();

private:
    // Child Widgets
    Glib::RefPtr<Gtk::Builder> menu_builder;

    // Main window
    Gtk::Stack stack1;
    Gtk::StackSwitcher switcher;
    Gtk::Picture background;
    Gtk::Overlay back_overlay, overlay;
    Gtk::Box btn_box, cfg_box;
    Gtk::Label api_label;
    Gtk::ComboBoxText combo;
    Gtk::Button btn_ver;

    // TitleBar and menu
    Gtk::HeaderBar header;
    Gtk::MenuButton menubtn;
    Gtk::PopoverMenu popover;
    void titlebar_init();

    // Css Style
    bool dark_mode;
    Glib::RefPtr<Gtk::CssProvider> provider;

    // Config Page
    MyPrefs *prefs;

    // Dialogs
    MsgBox msg_dialog;

    // Backgrounds
    void background1();
    void background2();
    void background3();

    // Version Configs
    struct tm *local;
    char api_version[57];
    json data;
    std::string config_longterm, config_stable, config_devel;
    void load_config();
    void config_dialog();

    // Signal Handlers
    void on_window_hide(Gtk::Window *window);
    void about_dialog();
    void main_releases();
};