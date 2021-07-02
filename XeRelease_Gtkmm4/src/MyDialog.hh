#pragma once

#include <gtkmm.h>

class MyDialog : public Gtk::Dialog{
public:
    MyDialog(Gtk::Window &parent);
    void set_filename(const char *filename);
    void set_msg(Glib::ustring msg);
protected:
    void on_response(int response_id) override;
private:
    //Child widgets
    Gtk::Label msg_label,label1;
    Gtk::Entry entry;
    Gtk::Box *box,vbox;
    //File Proprties
    char filename1[57];
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
