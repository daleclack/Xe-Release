#pragma once

#include <gtkmm.h>
#include <fstream>
#include "../json_nlohmann/json.hpp"

using json = nlohmann::json;

class MyDialog : public Gtk::Dialog{
public:
    MyDialog(BaseObjectType* cobject,const Glib::RefPtr<Gtk::Builder>& ref_builder);
    static MyDialog * create(Gtk::Window& parent);
protected:
    void on_response(int response_id) override;
private:
    Glib::RefPtr<Gtk::Builder> ref_Glade;
    //Child widgets
    Gtk::Entry * entry_lts,* entry_stable,* entry_dev,* entry_path;
    Gtk::Button * btnpath;

    //Strings to store path on Windows and Unix-Like systems
    std::string config_win32,config_unix;

    // Json data
    json data;

    //Signal Handlers
    Glib::RefPtr<Gtk::FileChooserNative> dialog;
    void btnpath_clicked();
    void dialog_response(int response_id);
};

class MsgBox : public Gtk::Dialog{
public:
    MsgBox(Gtk::Window &parent);
    void Init(Glib::ustring msg);
protected:
    //Signal Handler
    void on_response(int response_id) override;
private:
    //Child Widgets
    Gtk::Image image;
    Gtk::Label msg_label;
    Gtk::Box *vbox,hbox;
};

static inline bool unix_file_system_detected(){
#ifdef _WIN32
    return false;
#else
    return true;
#endif
}
