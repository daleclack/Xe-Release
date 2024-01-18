#include "MsgBox.hh"

MsgBox::MsgBox(Gtk::Window &parent)
    : vbox(Gtk::Orientation::VERTICAL, 5),
      hbox(Gtk::Orientation::HORIZONTAL, 5),
      btn_box(Gtk::Orientation::HORIZONTAL, 5),
      btn_ok("OK")
{
    // Initalize MsgBox
    set_icon_name("Xe-Release");
    set_default_size(300, 150);
    // add_button("OK", Gtk::ResponseType::OK);
    set_transient_for(parent);
    // Add Message and icon
    image.set_from_icon_name("Xe-Release");
    image.set_icon_size(Gtk::IconSize::LARGE);
    image.set_size_request(64, 64);
    // vbox = get_content_area();
    hbox.append(image);
    hbox.append(msg_label);

    // Add message box to the main box
    hbox.set_expand();
    hbox.set_halign(Gtk::Align::FILL);
    hbox.set_valign(Gtk::Align::FILL);
    vbox.append(hbox);
    vbox.set_margin(5);

    // Add button
    btn_box.append(btn_ok);
    btn_box.set_halign(Gtk::Align::END);
    btn_ok.set_halign(Gtk::Align::END);
    btn_ok.signal_clicked().connect(sigc::mem_fun(*this, &MsgBox::on_response));
    vbox.append(btn_box);

    set_child(vbox);
}

MsgBox::MsgBox()
    : vbox(Gtk::Orientation::VERTICAL, 5),
      hbox(Gtk::Orientation::HORIZONTAL, 5),
      btn_box(Gtk::Orientation::HORIZONTAL, 5),
      btn_ok("OK")
{
     // Initalize MsgBox
    set_icon_name("Xe-Release");
    set_default_size(300, 150);
    // add_button("OK", Gtk::ResponseType::OK);
    // set_transient_for(parent);
    // Add Message and icon
    image.set_from_icon_name("Xe-Release");
    image.set_icon_size(Gtk::IconSize::LARGE);
    image.set_size_request(64, 64);
    // vbox = get_content_area();
    hbox.append(image);
    hbox.append(msg_label);

    // Add message box to the main box
    hbox.set_expand();
    hbox.set_halign(Gtk::Align::FILL);
    hbox.set_valign(Gtk::Align::FILL);
    vbox.append(hbox);
    vbox.set_margin(5);

    // Add button
    btn_box.append(btn_ok);
    btn_box.set_halign(Gtk::Align::END);
    btn_ok.set_halign(Gtk::Align::END);
    btn_ok.signal_clicked().connect(sigc::mem_fun(*this, &MsgBox::on_response));
    vbox.append(btn_box);

    set_child(vbox);
}

void MsgBox::Init(Glib::ustring msg)
{
    msg_label.set_label(msg);
}

void MsgBox::on_response()
{
    hide();
}
