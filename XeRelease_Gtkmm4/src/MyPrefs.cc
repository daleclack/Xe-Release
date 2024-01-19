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
    btnadd->signal_clicked().connect(sigc::mem_fun(*this, &MyPrefs::btnadd_clicked));
    btnremove->signal_clicked().connect(sigc::mem_fun(*this, &MyPrefs::btnremove_clicked));
    btnpath->signal_clicked().connect(sigc::mem_fun(*this, &MyPrefs::btnpath_clicked));
    btnok->signal_clicked().connect(sigc::mem_fun(*this, &MyPrefs::btnok_clicked));
    btncancel->signal_clicked().connect(sigc::mem_fun(*this, &MyPrefs::btnreset_clicked));

    // Create the list store
    ver_list = Gio::ListStore<ModelColumns>::create();
    selection = Gtk::NoSelection::create(ver_list);

    // List content for test
    ver_list->append(ModelColumns::create("Longterm", "5.15", 0));
    ver_list->append(ModelColumns::create("Stable", "9.1", 1));
    ver_list->append(ModelColumns::create("Develop", "-1", 1));
    version_view.set_model(selection);

    // Add Column View
    version_sw->set_child(version_view);

    // Add branch column
    branch_factory = Gtk::SignalListItemFactory::create();
    branch_factory->signal_bind().connect(sigc::mem_fun(*this, &MyPrefs::bind_branch));
    branch_factory->signal_setup().connect(sigc::mem_fun(*this, &MyPrefs::setup_branch));
    branch_column = Gtk::ColumnViewColumn::create("Branch", branch_factory);
    version_view.append_column(branch_column);

    // Add version column
    version_factory = Gtk::SignalListItemFactory::create();
    version_factory->signal_bind().connect(sigc::mem_fun(*this, &MyPrefs::bind_version));
    version_factory->signal_setup().connect(sigc::mem_fun(*this, &MyPrefs::setup_version));
    version_column = Gtk::ColumnViewColumn::create("Version", version_factory);
    version_view.append_column(version_column);

    // Create list for mode column
    drop_list = Gtk::StringList::create({"Mode0", "Mode1"});

    // Add mode column
    mode_factory = Gtk::SignalListItemFactory::create();
    mode_factory->signal_bind().connect(sigc::mem_fun(*this, &MyPrefs::bind_mode));
    mode_factory->signal_setup().connect(sigc::mem_fun(*this, &MyPrefs::setup_mode));
    mode_column = Gtk::ColumnViewColumn::create("Mode", mode_factory);
    version_view.append_column(mode_column);
}

void MyPrefs::btnok_clicked()
{
    // Save Configs to a file
    Glib::ustring config;
    std::fstream outfile;
    
    // Config for Branches, Versions and Modes
    str_vec branches, versions;
    std::vector<guint> modes;

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
    // Open the config file
    outfile.open("xe_config.json", std::ios_base::out);
    /*OutPut contents to the file
        Simple Contents of xe_config:
        {
            "Branches":[],
            "Versions":[],
            "Modes":[],
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
                    "Branches":[],
                    "Versions":[],
                    "Modes":[],
                    "Release_Path_Unix":"",
                    "Release_Path_Win32":"",
                    "Release_Path_Darwin":"",
                    "dark_mode":false,
                    "background":2
                }
            )");

        // Get Branchs, Versions and Modes
        for(int i = 0; i < ver_list->get_n_items(); i++)
        {
            // Get Item
            auto item = ver_list->get_item(i);

            // Get properities
            branches.push_back(item->get_branch_str().c_str());
            versions.push_back(item->get_version_str().c_str());
            modes.push_back(item->get_branch_mode());
        }

        // Load config to json file
        out_data["Branches"] = branches;
        out_data["Versions"] = versions;
        out_data["Modes"] = modes;
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

void MyPrefs::reset_entries()
{
    str_vec branchs_vec, versions_vec;
    // Read json data
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

void MyPrefs::setup_branch(const Glib::RefPtr<Gtk::ListItem> &item)
{
    // Set label for item
    item->set_child(*Gtk::make_managed<Gtk::Entry>());
}

void MyPrefs::bind_branch(const Glib::RefPtr<Gtk::ListItem> &item)
{
    // Get position
    auto pos = item->get_position();

    // Get Entry
    auto entry = dynamic_cast<Gtk::Entry *>(item->get_child());
    if (!entry)
    {
        return;
    }

    // Bind text
    auto item1 = ver_list->get_item(pos);
    entry->set_text(item1->get_branch_str());
    Glib::Binding::bind_property(item1->property_branch(), entry->property_text(),
                                 Glib::Binding::Flags::BIDIRECTIONAL);
}

void MyPrefs::setup_version(const Glib::RefPtr<Gtk::ListItem> &item)
{
    // Set label for item
    item->set_child(*Gtk::make_managed<Gtk::Entry>());
}

void MyPrefs::bind_version(const Glib::RefPtr<Gtk::ListItem> &item)
{
    // Get position
    auto pos = item->get_position();

    // Get Entry
    auto entry = dynamic_cast<Gtk::Entry *>(item->get_child());
    if (!entry)
    {
        return;
    }

    // Bind text
    auto item1 = ver_list->get_item(pos);
    entry->set_text(item1->get_version_str());
    Glib::Binding::bind_property(item1->property_version(), entry->property_text(),
                                 Glib::Binding::Flags::BIDIRECTIONAL);
}

void MyPrefs::setup_mode(const Glib::RefPtr<Gtk::ListItem> &item)
{
    // Add dropdown for mode selection
    auto dropdown_mode = Gtk::make_managed<Gtk::DropDown>(drop_list);
    item->set_child(*dropdown_mode);
}

void MyPrefs::bind_mode(const Glib::RefPtr<Gtk::ListItem> &item)
{
    // Get Position
    auto pos = item->get_position();

    // Get Dropdown widget
    auto dropdown_mode = dynamic_cast<Gtk::DropDown *>(item->get_child());
    if (!dropdown_mode)
    {
        return;
    }

    // Bind mode
    auto item1 = ver_list->get_item(pos);
    dropdown_mode->set_selected(item1->get_branch_mode());
    Glib::Binding::bind_property(item1->property_branch_mode(), dropdown_mode->property_selected(),
                                 Glib::Binding::Flags::BIDIRECTIONAL);
}

void MyPrefs::btnadd_clicked()
{
    // Append a item to the list
    ver_list->append(ModelColumns::create("<empty>", "<empty>", 1));
}

void MyPrefs::btnremove_clicked()
{
    // Get Position of item
    auto pos = ver_list->get_n_items() - 1;

    // Remove item
    ver_list->remove(pos);
}

int MyPrefs::get_background_id()
{
    // The id for background
    return background_id;
}

MyListStore MyPrefs::get_model()
{
    // The store for the dropdown and list
    return ver_list;
}

void MyPrefs::save_config_now()
{
    btnok_clicked();
}
