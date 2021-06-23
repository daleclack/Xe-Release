#include "MyDialog.hh"

MsgBox::MsgBox(Gtk::Window &parent)
:hbox(Gtk::ORIENTATION_HORIZONTAL,5)
{
    //Initalize MsgBox
    set_icon_name("Xe-Release");
    set_default_size(300,150);
    add_button("OK",Gtk::RESPONSE_OK);
    set_transient_for(parent);
    //Add Message
    image.set_from_icon_name("Xe-Release",Gtk::ICON_SIZE_DIALOG);
    vbox=get_content_area();
    hbox.pack_start(image,Gtk::PACK_SHRINK);
    hbox.pack_start(msg_label,Gtk::PACK_SHRINK);
    vbox->pack_start(hbox);
}

void MsgBox::Init(Glib::ustring msg){
    msg_label.set_label(msg);
}

void MsgBox::on_response(int response_id){
    hide();
}
