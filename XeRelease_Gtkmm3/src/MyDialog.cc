#include "MyDialog.hh"
#include "../cfgfile2/cfgfile.hh"

MyDialog::MyDialog(BaseObjectType* cobject,const Glib::RefPtr<Gtk::Builder>& ref_builder)
:Gtk::Dialog(cobject),
ref_Glade(ref_builder)
{
    //Initalize
    set_icon_name("XeRelease");
    //Get Widgets
    ref_builder->get_widget("entry_lts",entry_lts);
    ref_builder->get_widget("entry_stable",entry_stable);
    ref_builder->get_widget("entry_dev",entry_dev);
    //Read Configs
    std::string config;
    if(readCfgFile("xe_config","Longterm",config)){
        entry_lts->set_text(config);
    }
    if(readCfgFile("xe_config","Stable",config)){
        entry_stable->set_text(config);
    }
    if(readCfgFile("xe_config","Develop",config)){
        entry_dev->set_text(config);
    }
}

void MyDialog::on_response(int response_id){
    //Save Configs to a file
    if(response_id == Gtk::RESPONSE_OK){
        Glib::ustring config;
        std::fstream outfile;
        outfile.open("xe_config",std::ios_base::out);
        if(outfile.is_open()){
            outfile<<"This is the config file of Xe Release"<<std::endl;
            outfile<<"See more on github.com/daleclack/Xe-Release"<<std::endl;
            outfile<<std::endl;
            config=entry_lts->get_text();
            outfile<<"Longterm="<<config<<std::endl;
            config=entry_stable->get_text();
            outfile<<"Stable="<<config<<std::endl;
            config=entry_dev->get_text();
            outfile<<"Develop="<<config<<std::endl;
        }
        outfile.close();
    }
    hide();
}

MyDialog * MyDialog::create(Gtk::Window& parent){   
    //Create a dialog
    auto builder=Gtk::Builder::create_from_resource("/XeRelease/prefs.ui");
    
    MyDialog * dialog=nullptr;
    builder->get_widget_derived("prefs",dialog);
    dialog->set_transient_for(parent);

    return dialog;
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
