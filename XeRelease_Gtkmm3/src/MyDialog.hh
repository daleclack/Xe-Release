#pragma once

#include <gtkmm.h>

class MyDialog : public Gtk::Dialog{

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
