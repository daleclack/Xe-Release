#include "MyPrefs.hh"

MyPrefs::MyPrefs(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &ref_builder)
    : Gtk::Box(cobject),
      ref_Glade(ref_builder)
{
    // Get Widgets
    version_sw = ref_builder->get_widget<Gtk::ScrolledWindow>("version_sw");
    btnadd = ref_builder->get_widget<Gtk::Button>("btn_add");
    btnremove = ref_builder->get_widget<Gtk::Button>("btn_remove");
    entry_path = ref_builder->get_widget<Gtk::Entry>("entry_path");
    btnpath = ref_builder->get_widget<Gtk::Button>("btnpath");
    btnok = ref_builder->get_widget<Gtk::Button>("btn_ok");
    btncancel = ref_builder->get_widget<Gtk::Button>("btn_cancel");

    // Connect Signal
    btnpath->signal_clicked().connect(sigc::mem_fun(*this, &MyPrefs::btnpath_clicked));
    btnok->signal_clicked().connect(sigc::mem_fun(*this, &MyPrefs::btnok_clicked));
    btncancel->signal_clicked().connect(sigc::mem_fun(*this, &MyPrefs::btnreset_clicked));
}

void MyPrefs::btnok_clicked()
{
    // Save Configs to a file
    Glib::ustring config;
    std::fstream outfile;

    // Initalize Config for path
    switch (get_os_type())
    {
    case OS_Type::Linux:
        config_unix = entry_path->get_text();
        break;
    case OS_Type::Windows:
        config_win32 = entry_path->get_text();
        break;
    case OS_Type::Darwin:
        config_darwin = entry_path->get_text();
        break;
    }
    // if (get_os_type() == OS_Type::Linux)
    // {
    //     config_unix = entry_path->get_text();
    // }
    // else
    // {

    //     config_win32 = entry_path->get_text();
    // }
    // Open the config file
    outfile.open("xe_config.json", std::ios_base::out);
    /*OutPut contents to the file
        Simple Contents of xe_config:
        {
            "Longterm":"x.x",
            "Stable":"x.x",
            "Develop":"x.x",
            "dark_mode":false,
            "background":0
            "Release_Path_Unix":"",
            "Release_Path_Win32":"",
            "Release_Path_Darwin":""
        }
    */
    if (outfile.is_open())
    {
        // BackUp the original data
        data_backup = data;

        // Create json object
        json out_data = json::parse(R"(
                {
                    "Longterm":"",
                    "Stable":"",
                    "Develop":"",
                    "Release_Path_Unix":"",
                    "Release_Path_Win32":"",
                    "Release_Path_Darwin":"",
                    "dark_mode":false,
                    "background":2
                }
            )");

        // Load config to json file
        out_data["Release_Path_Unix"] = config_unix;
        out_data["Release_Path_Win32"] = config_win32;
        out_data["Release_Path_Darwin"] = config_darwin;
        out_data["dark_mode"] = dark_mode;
        out_data["background"] = background_id;
        outfile << out_data;

        // Set Current json data
        data = out_data;

        // Show Dialog
        msg_dialog1.Init("Config File Saved!");
        msg_dialog1.present();
    }
    outfile.close();
}

void MyPrefs::btnreset_clicked()
{
    // Restore the backup data
    data = data_backup;

    // Reset content of entries
    reset_entries();

    // Show Dialog
    msg_dialog1.Init("Config Reseted!\n Press \"OK\" to save.");
    msg_dialog1.present();
}

void MyPrefs::init_json_data(json &data1)
{
    // Read Configs
    // Open json file
    if (!data1.empty())
    {
        data = data1;
        // Set the content of entry
        reset_entries();
    }
}

void MyPrefs::reset_entries()
{
    std::string config_longterm, config_stable, config_devel;
    // Read json data
    config_longterm = data["Longterm"];
    config_stable = data["Stable"];
    config_devel = data["Develop"];
    config_unix = data["Release_Path_Unix"];
    config_win32 = data["Release_Path_Win32"];
    config_darwin = data["Release_Path_Darwin"];
    dark_mode = data["dark_mode"];
    background_id = data["background"];

    // Set text from json file data

    // Use different path for Linux filesystem and Windows
    switch (get_os_type())
    {
    case OS_Type::Linux:
        entry_path->set_text(config_unix);
        break;
    case OS_Type::Windows:
        entry_path->set_text(config_win32);
        break;
    case OS_Type::Darwin:
        entry_path->set_text(config_darwin);
        break;
    }
    // if (get_os_type() == OS_Type::Linux)
    // {
    //     entry_path->set_text(config_unix);
    // }
    // else
    // {
    //     entry_path->set_text(config_win32);
    // }
}

MyPrefs *MyPrefs::create()
{
    // Create the config widget
    auto builder = Gtk::Builder::create_from_resource("/org/gtk/daleclack/prefs.ui");

    MyPrefs *box = nullptr;
    box = Gtk::Builder::get_widget_derived<MyPrefs>(builder, "prefs");

    return box;
}

void MyPrefs::set_parent_win(Gtk::Window *parent)
{
    parent_win = parent;
    msg_dialog1.set_transient_for(*parent);
}

void MyPrefs::set_dark_mode(bool dark_mode_enabled)
{
    // Put the config of dark mode to the class
    dark_mode = dark_mode_enabled;
}

void MyPrefs::save_config_now()
{
    // Save config when the dark mode config is modified
    btnok_clicked();
}

void MyPrefs::btnpath_clicked()
{
    // Create a Dialog
    dialog = Gtk::FileChooserNative::create("Select a folder", *parent_win,
                                            Gtk::FileChooser::Action::SELECT_FOLDER, "OK", "Cancel");

    dialog->signal_response().connect(sigc::mem_fun(*this, &MyPrefs::dialog_response));

    dialog->show();
}

void MyPrefs::dialog_response(int response_id)
{
    if (response_id == Gtk::ResponseType::ACCEPT)
    {
        auto file = dialog->get_file();
        Glib::ustring path = file->get_path();
        entry_path->set_text(path);
        file.reset();
    }
    dialog.reset();
}

