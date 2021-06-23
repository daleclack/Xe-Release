#include "MyDialog.hh"
#include <cstdio>

MyDialog::MyDialog(Gtk::Window &parent)
:label2("    ")
{
    //Initalize MsgBox
    set_icon_name("Xe-Release");
    set_default_size(300,150);
    add_button("OK",Gtk::RESPONSE_OK);
    add_button("Cancel",Gtk::RESPONSE_CANCEL);
    set_transient_for(parent);
    //Add Widgets
    entry.set_text("default config");
    entry.signal_activate().connect(sigc::bind(sigc::mem_fun(*this,&MyDialog::response),Gtk::RESPONSE_OK));
    vbox=get_content_area();
    vbox->pack_start(msg_label,Gtk::PACK_SHRINK);
    vbox->pack_start(label2,Gtk::PACK_SHRINK);
    vbox->pack_start(entry,Gtk::PACK_SHRINK);
}

void MyDialog::set_filename(const char *filename){
    strcpy(filename1,filename);
}

void MyDialog::set_msg(Glib::ustring msg){
    msg_label.set_label(msg);
}

void MyDialog::on_response(int response_id){
    //Save Text into file
    if(response_id==Gtk::RESPONSE_OK){
        FILE *fp=fopen(filename1,"wt+");
        Glib::ustring str=entry.get_text();
        fprintf(fp,"%s",str.c_str());
        fclose(fp);
    }
    entry.set_text("default config");
    hide();
}

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
