#include "MyPrefs.hh"

MyPrefs::MyPrefs(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &ref_builder)
    : Gtk::Box(cobject),
      ref_Glade(ref_builder)
{
    // Get Widgets
    entry_lts = ref_builder->get_widget<Gtk::Entry>("entry_lts");
    entry_stable = ref_builder->get_widget<Gtk::Entry>("entry_stable");
    entry_dev = ref_builder->get_widget<Gtk::Entry>("entry_dev");
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
                    "dark_mode":false  
                }
            )");

        // Load config to json file
        out_data["Longterm"] = entry_lts->get_text();
        out_data["Stable"] = entry_stable->get_text();
        out_data["Develop"] = entry_dev->get_text();
        out_data["Release_Path_Unix"] = config_unix;
        out_data["Release_Path_Win32"] = config_win32;
        out_data["Release_Path_Darwin"] = config_darwin;
        out_data["dark_mode"] = dark_mode;
        outfile << out_data;

        // Set Current json data
        data = out_data;

        // Show Dialog
        msg_dialog1.Init("Config File Saved!");
        msg_dialog1.show();
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
    msg_dialog1.show();
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

    // Set text from json file data
    entry_lts->set_text(config_longterm);
    entry_stable->set_text(config_stable);
    entry_dev->set_text(config_devel);

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

MsgBox::MsgBox(Gtk::Window &parent)
    : hbox(Gtk::Orientation::HORIZONTAL, 5)
{
    // Initalize MsgBox
    set_icon_name("Xe-Release");
    set_default_size(300, 150);
    add_button("OK", Gtk::ResponseType::OK);
    set_transient_for(parent);
    // Add Message
    image.set_from_icon_name("Xe-Release");
    image.set_icon_size(Gtk::IconSize::LARGE);
    image.set_size_request(64, 64);
    vbox = get_content_area();
    hbox.append(image);
    hbox.append(msg_label);
    hbox.set_expand();
    hbox.set_halign(Gtk::Align::FILL);
    hbox.set_valign(Gtk::Align::FILL);
    vbox->append(hbox);
    vbox->set_margin(5);
}

MsgBox::MsgBox()
{
    // Initalize MsgBox
    set_icon_name("Xe-Release");
    set_default_size(300, 150);
    add_button("OK", Gtk::ResponseType::OK);
    // Add Message
    image.set_from_icon_name("Xe-Release");
    image.set_icon_size(Gtk::IconSize::LARGE);
    image.set_size_request(64, 64);
    vbox = get_content_area();
    hbox.append(image);
    hbox.append(msg_label);
    hbox.set_expand();
    hbox.set_halign(Gtk::Align::FILL);
    hbox.set_valign(Gtk::Align::FILL);
    vbox->append(hbox);
    vbox->set_margin(5);
}

void MsgBox::Init(Glib::ustring msg)
{
    msg_label.set_label(msg);
}

void MsgBox::on_response(int response_id)
{
    hide();
}
