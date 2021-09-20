#pragma once

#include <gtkmm.h>
#include <fstream>
#include "../cfgfile2/cfgfile.hh"

class MyDialog : public Gtk::Dialog{
public:
    MyDialog(BaseObjectType* cobject,const Glib::RefPtr<Gtk::Builder>& ref_builder);
    static MyDialog * create(Gtk::Window& parent);
protected:
    void on_response(int response_id) override;
private:
    Glib::RefPtr<Gtk::Builder> ref_Glade;
    Gtk::Entry * entry_lts,* entry_stable,* entry_dev;
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
