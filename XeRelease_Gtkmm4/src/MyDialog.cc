#include "MyDialog.hh"
#include <cstdio>

MyDialog::MyDialog(Gtk::Window &parent)
:label1("    "),
vbox(Gtk::Orientation::VERTICAL,5)
{
    //Initalize MsgBox
    set_icon_name("Xe-Release");
    set_default_size(300,150);
    add_button("OK",Gtk::ResponseType::OK);
    add_button("Cancel",Gtk::ResponseType::CANCEL);
    set_default_response(Gtk::ResponseType::OK);
    set_transient_for(parent);
    //Add Widgets
    entry.set_text("default config");
    entry.set_activates_default();
    box=get_content_area();
    vbox.append(label1);
    vbox.append(msg_label);
    vbox.append(entry);
    vbox.set_hexpand();
    vbox.set_vexpand();
    box->append(vbox);
}

void MyDialog::set_filename(const char *filename){
    strcpy(filename1,filename);
}

void MyDialog::set_msg(Glib::ustring msg){
    msg_label.set_label(msg);
}

void MyDialog::on_response(int response_id){
    //Save Text into file
    if(response_id==Gtk::ResponseType::OK){
        FILE *fp=fopen(filename1,"wt+");
        Glib::ustring str=entry.get_text();
        fprintf(fp,"%s",str.c_str());
        fclose(fp);
    }
    entry.set_text("default config");
    hide();
}

MsgBox::MsgBox(Gtk::Window &parent)
:hbox(Gtk::Orientation::HORIZONTAL,5)
{
    //Initalize MsgBox
    set_icon_name("Xe-Release");
    set_default_size(300,150);
    add_button("OK",Gtk::ResponseType::OK);
    set_transient_for(parent);
    //set_margin(10);
    //Add Message
    image.set_size_request(48,48);
    image.set_from_icon_name("Xe-Release");
    image.set_pixel_size(48);
    vbox=get_content_area();
    hbox.append(image);
    hbox.append(msg_label);
    hbox.set_hexpand();
    hbox.set_vexpand();
    hbox.set_margin(10);
    vbox->append(hbox);
}

void MsgBox::Init(Glib::ustring msg){
    msg_label.set_label(msg);
}

void MsgBox::on_response(int response_id){
    hide();
}
