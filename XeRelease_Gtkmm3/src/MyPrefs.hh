#pragma once

#include <gtkmm.h>
#include <fstream>
#include "../json_nlohmann/json.hpp"

using json = nlohmann::json;

class MyPrefs : public Gtk::Box
{
public:
    MyPrefs(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &ref_builder);
    static MyPrefs *create();
    void set_parent_win(Gtk::Window *parent);
    void init_json_data(json &data1);

private:
    Glib::RefPtr<Gtk::Builder> ref_Glade;
    // Parent Window
    Gtk::Window *parent_win;

    // Child widgets
    Gtk::Entry *entry_lts, *entry_stable, *entry_dev, *entry_path;
    Gtk::Button *btnpath, *btnok;

    // Strings to store path on Windows and Unix-Like systems
    std::string config_win32, config_unix;

    // Signal Handlers
    Glib::RefPtr<Gtk::FileChooserNative> dialog;
    void btnpath_clicked();
    void dialog_response(int response_id);
    void btnok_clicked();
};

class MsgBox : public Gtk::Dialog
{
public:
    MsgBox(Gtk::Window &parent);
    void Init(Glib::ustring msg);

protected:
    // Signal Handler
    void on_response(int response_id) override;

private:
    // Child Widgets
    Gtk::Image image;
    Gtk::Label msg_label;
    Gtk::Box *vbox, hbox;
};

static inline bool unix_file_system_detected()
{
#ifdef _WIN32
    return false;
#else
    return true;
#endif
}
