#pragma once

#include <gtkmm.h>

class MsgBox : public Gtk::Window
{
public:
    MsgBox(Gtk::Window &parent);
    MsgBox();
    void Init(Glib::ustring msg);

private:
    // Child Widgets
    Gtk::Image image;
    Gtk::Label msg_label;
    Gtk::Box vbox, hbox, btn_box;
    Gtk::Button btn_ok;

    // Signal Handler
    void on_response();
};
