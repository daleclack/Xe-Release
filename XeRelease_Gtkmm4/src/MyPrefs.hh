#pragma once

#include <gtkmm.h>
#include <fstream>
#include "json_nlohmann/json.hpp"
#include "config.hh"
#include "MsgBox.hh"

using json = nlohmann::json;

class MyPrefs : public Gtk::Box
{
public:
    MyPrefs(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &ref_builder);
    static MyPrefs *create();
    void set_parent_win(Gtk::Window *parent);
    void init_json_data(json &data1);
    void save_config_now();
    void set_dark_mode(bool dark_mode_enabled);
    int background_id = 3;

private:
    Glib::RefPtr<Gtk::Builder> ref_Glade;
    // Parent Window
    Gtk::Window *parent_win;

    // Child widgets
    Gtk::ScrolledWindow *version_sw;
    Gtk::Button *btnadd, *btnremove;
    Gtk::Entry *entry_path;
    Gtk::Button *btnpath, *btnok, *btncancel;
    bool dark_mode;

    // Strings to store path on Windows and Unix-Like systems
    std::string config_win32, config_unix, config_darwin;
    json data, data_backup;

    // Messagebox
    MsgBox msg_dialog1;

    // Signal Handlers
    Glib::RefPtr<Gtk::FileChooserNative> dialog;
    void btnpath_clicked();
    void dialog_response(int response_id);
    void btnok_clicked();
    void btnreset_clicked();
    void reset_entries();
};

// static inline bool unix_file_system_detected()
// {
// #ifdef _WIN32
//     return false;
// #else
//     return true;
// #endif
// }
