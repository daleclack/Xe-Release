#pragma once

#include <gtkmm.h>
#include <fstream>
#include "json_nlohmann/json.hpp"
#include "config.hh"
#include "MsgBox.hh"
#include "ModelColumns.hh"

using json = nlohmann::json;
typedef std::vector<std::string> str_vec;
typedef Glib::RefPtr<Gio::ListStore<ModelColumns>> MyListStore;

class MyPrefs : public Gtk::Box
{
public:
    MyPrefs(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &ref_builder);
    static MyPrefs *create();
    void set_parent_win(Gtk::Window *parent);

    // xe_config file management
    void load_config();

    // Background preferences
    void set_background_id(int back_id);
    int get_background_id();

    MyListStore get_model();
    int background_id = 3;

private:
    Glib::RefPtr<Gtk::Builder> ref_Glade;
    // Parent Window
    Gtk::Window *parent_win;

    // Child widgets
    Gtk::ScrolledWindow *version_sw;
    Gtk::ColumnView version_view;
    Gtk::Button *btnadd, *btnremove;
    Gtk::Entry *entry_path;
    Gtk::Button *btnpath, *btnok, *btncancel;
    bool dark_mode;

    // The Column View for versions
    MyListStore ver_list;
    Glib::RefPtr<Gtk::NoSelection> selection;
    Glib::RefPtr<Gtk::StringList> drop_list;

    // Factory to renderer branch string
    Glib::RefPtr<Gtk::ColumnViewColumn> branch_column;
    Glib::RefPtr<Gtk::SignalListItemFactory> branch_factory;
    void setup_branch(const Glib::RefPtr<Gtk::ListItem> &item);
    void bind_branch(const Glib::RefPtr<Gtk::ListItem> &item);

    // Factory to renderer version string
    Glib::RefPtr<Gtk::ColumnViewColumn> version_column;
    Glib::RefPtr<Gtk::SignalListItemFactory> version_factory;
    void setup_version(const Glib::RefPtr<Gtk::ListItem> &item);
    void bind_version(const Glib::RefPtr<Gtk::ListItem> &item);

    // Factory to renderer the mode selection
    Glib::RefPtr<Gtk::ColumnViewColumn> mode_column;
    Glib::RefPtr<Gtk::SignalListItemFactory> mode_factory;
    void setup_mode(const Glib::RefPtr<Gtk::ListItem> &item);
    void bind_mode(const Glib::RefPtr<Gtk::ListItem> &item);

    // Strings to store path on Windows and Unix-Like systems
    std::string config_win32, config_unix, config_darwin;
    json data, data_backup;

    // Messagebox
    MsgBox msg_dialog1;

    // Signal Handlers
    Glib::RefPtr<Gtk::FileChooserNative> dialog;
    void btnadd_clicked();
    void btnremove_clicked();
    void btnpath_clicked();
    void dialog_response(int response_id);
    void btnok_clicked();
    void btnreset_clicked();
    void apply_config();
};

// static inline bool unix_file_system_detected()
// {
// #ifdef _WIN32
//     return false;
// #else
//     return true;
// #endif
// }
