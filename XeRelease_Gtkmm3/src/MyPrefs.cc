#include "MyPrefs.hh"

MyPrefs::MyPrefs(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &ref_builder)
    : Gtk::Box(cobject),
      ref_Glade(ref_builder)
{
    // // Initalize
    // set_icon_name("XeRelease");
    // Get Widgets
    ref_builder->get_widget("entry_lts", entry_lts);
    ref_builder->get_widget("entry_stable", entry_stable);
    ref_builder->get_widget("entry_dev", entry_dev);
    ref_builder->get_widget("entry_path", entry_path);
    ref_builder->get_widget("btnpath", btnpath);
    ref_builder->get_widget("btn_ok", btnok);

    // json_file.close();

    // std::string config;
    // if(readCfgFile("xe_config","Longterm",config)){
    //     entry_lts->set_text(config);
    // }
    // if(readCfgFile("xe_config","Stable",config)){
    //     entry_stable->set_text(config);
    // }
    // if(readCfgFile("xe_config","Develop",config)){
    //     entry_dev->set_text(config);
    // }
    // readCfgFile("xe_config","Release_Path_Unix",config_unix);
    // readCfgFile("xe_config","Release_Path_Win32",config_win32);

    // Connect Signal
    btnpath->signal_clicked().connect(sigc::mem_fun(*this, &MyPrefs::btnpath_clicked));
    btnok->signal_clicked().connect(sigc::mem_fun(*this, &MyPrefs::btnok_clicked));
}

void MyPrefs::btnok_clicked()
{
    // Save Configs to a file
    Glib::ustring config;
    std::fstream outfile;

    // Initalize Config for path
    if (unix_file_system_detected())
    {
        config_unix = entry_path->get_text();
    }
    else
    {

        config_win32 = entry_path->get_text();
    }
    // Open the config file
    outfile.open("xe_config.json", std::ios_base::out);
    /*OutPut contents to the file
        Simple Contents of xe_config:
        Longterm=x.x
        Stable=x.x
        Develop=x.x
        Release_Path_Unix=/xxx/xxx
        Release_Path_Win32=X:\xxx\xxx
    */
    if (outfile.is_open())
    {
        // Create json object
        json out_data = json::parse(R"(
                {
                    "Longterm":"",
                    "Stable":"",
                    "Develop":"",
                    "Release_Path_Unix":"",
                    "Release_Path_Win32":""
                }
            )");

        // Load config to json file
        out_data["Longterm"] = entry_lts->get_text();
        out_data["Stable"] = entry_stable->get_text();
        out_data["Develop"] = entry_dev->get_text();
        out_data["Release_Path_Unix"] = config_unix;
        out_data["Release_Path_Win32"] = config_win32;
        outfile << out_data;
        // outfile<<"This is the config file of Xe Release"<<std::endl;
        // outfile<<"See more on github.com/daleclack/Xe-Release"<<std::endl;
        // outfile<<std::endl;
        // config=entry_lts->get_text();
        // outfile<<"Longterm="<<config<<std::endl;
        // config=entry_stable->get_text();
        // outfile<<"Stable="<<config<<std::endl;
        // config=entry_dev->get_text();
        // outfile<<"Develop="<<config<<std::endl;
        // outfile<<"Release_Path_Unix="<<config_unix<<std::endl;
        // outfile<<"Release_Path_Win32="<<config_win32<<std::endl;
    }
    outfile.close();
}

void MyPrefs::init_json_data(json &data1)
{
    // Read Configs
    std::string config_longterm, config_stable, config_devel;
    // Open json file
    if (!data1.empty())
    {
        // Read data from json file
        // data = json::parse(json_file);
        config_longterm = data1["Longterm"];
        config_stable = data1["Stable"];
        config_devel = data1["Develop"];
        config_unix = data1["Release_Path_Unix"];
        config_win32 = data1["Release_Path_Win32"];

        // Set text from json file data
        entry_lts->set_text(config_longterm);
        entry_stable->set_text(config_stable);
        entry_dev->set_text(config_devel);

        // Use different path for Linux filesystem and Windows
        if (unix_file_system_detected())
        {
            entry_path->set_text(config_unix);
        }
        else
        {
            entry_path->set_text(config_win32);
        }
    }
}

MyPrefs *MyPrefs::create()
{
    // Create the config widget
    auto builder = Gtk::Builder::create_from_resource("/org/gtk/daleclack/prefs.ui");

    MyPrefs *box = nullptr;
    builder->get_widget_derived("prefs", box);

    return box;
}

void MyPrefs::set_parent_win(Gtk::Window *parent){
    parent_win = parent;
}

void MyPrefs::btnpath_clicked()
{
    // Create a Dialog
    dialog = Gtk::FileChooserNative::create("Select a folder", *parent_win,
                                            Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER, "OK", "Cancel");

    dialog->signal_response().connect(sigc::mem_fun(*this, &MyPrefs::dialog_response));

    dialog->show();
}

void MyPrefs::dialog_response(int response_id)
{
    if (response_id == Gtk::RESPONSE_ACCEPT)
    {
        Glib::ustring path = dialog->get_filename();
        entry_path->set_text(path);
    }
    dialog.reset();
}

MsgBox::MsgBox(Gtk::Window &parent)
    : hbox(Gtk::ORIENTATION_HORIZONTAL, 5)
{
    // Initalize MsgBox
    set_icon_name("Xe-Release");
    set_default_size(300, 150);
    add_button("OK", Gtk::RESPONSE_OK);
    set_transient_for(parent);
    // Add Message
    image.set_from_icon_name("Xe-Release", Gtk::ICON_SIZE_DIALOG);
    vbox = get_content_area();
    hbox.pack_start(image, Gtk::PACK_SHRINK);
    hbox.pack_start(msg_label, Gtk::PACK_SHRINK);
    vbox->pack_start(hbox);
}

void MsgBox::Init(Glib::ustring msg)
{
    msg_label.set_label(msg);
}

void MsgBox::on_response(int response_id)
{
    hide();
}
