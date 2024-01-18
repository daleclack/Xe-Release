#include "MyWin.hh"
#include "img7.xpm"
#include "winpe.xpm"
#include "fly.xpm"
#include "xeapi.hh"
#include "xerelease.hh"
#include "config.hh"
#include <cstdio>

enum Releases
{
    LTS,
    Stable,
    Dev
};

MyWin::MyWin()
    : btn_box(Gtk::Orientation::VERTICAL, 5),
      cfg_box(Gtk::Orientation::VERTICAL, 5),
      btn_ver("Xe-Ver"),
      msg_dialog(*this)
{
    // Initalize window
    set_icon_name("Xe-Release");
    set_default_size(640, 360);
    titlebar_init();

    // Set Background Image
    back_overlay.set_child(background);
    background.set_size_request(640, 360);
    back_overlay.set_halign(Gtk::Align::FILL);
    back_overlay.set_valign(Gtk::Align::FILL);
    back_overlay.set_expand();

    // Get Local time
    time_t t;
    t = time(NULL);
    local = localtime(&t);

    // Initalize Api Label
    snprintf(api_version, 57, "Xe Api Version:%d", xeapi1(local));
    api_label.set_label(api_version);

    // Initalize combobox
    combo_list = Gtk::StringList::create();
    combo_list->append("Longterm");
    combo_list->append("Stable");
    combo_list->append("Development");
    drop_down.set_model(combo_list);
    drop_down.set_selected(1);

    // Add Main Controls
    btn_box.set_halign(Gtk::Align::CENTER);
    btn_box.set_valign(Gtk::Align::CENTER);
    btn_box.append(api_label);
    btn_box.append(drop_down);
    btn_box.append(btn_ver);
    btn_box.set_opacity(0.7);
    overlay.add_overlay(btn_box);

    // Show everything
    set_child(back_overlay);
    back_overlay.add_overlay(stack1);
    stack1.add(overlay, "main_page", "WelCome");

    // Add Config Page
    prefs = MyPrefs::create();
    prefs->set_parent_win(this);
    load_config();
    prefs->init_json_data(data);
    cfg_box.append(*prefs);
    // cfg_box.set_hexpand();
    cfg_box.set_halign(Gtk::Align::CENTER);
    cfg_box.set_valign(Gtk::Align::CENTER);
    cfg_box.set_opacity(0.7);
    stack1.add(cfg_box, "config_page", "Config");

    // Set background from the config
    // The vaild background id is 1 to 3
    // else the background 3 will load
    switch (back_id)
    {
    case 1:
        background1();
        break;
    case 2:
        background2();
        break;
    case 3:
        background3();
        break;
    default:
        background3();
    }

    // Create Style for widgets
    // Due to the changes of Gtk4.10, the way to load custom style is changed
    provider = Gtk::CssProvider::create();

    // Only one css file is needed to load
    provider->load_from_resource("/org/gtk/daleclack/style.css");

    // Set css class of widget instead of loading different css files
    if (get_dark_mode())
    {
        btn_box.add_css_class("style_dark");
        prefs->add_css_class("style_dark");
    }
    else
    {
        btn_box.add_css_class("style_light");
        prefs->add_css_class("style_light");
    }

    // Apply css class for widgets
    Gtk::CssProvider::add_provider_for_display(btn_box.get_display(), provider,
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    Gtk::CssProvider::add_provider_for_display(prefs->get_display(), provider,
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    switcher.set_stack(stack1);

    // Connect Signals
    btn_ver.signal_clicked().connect(sigc::mem_fun(*this, &MyWin::main_releases));
}

bool MyWin::get_dark_mode()
{
    // Get Icon theme
    Glib::ustring icon_theme, tmp_str;
    auto display = get_display();
    Glib::RefPtr<Gtk::IconTheme> theme1 = Gtk::IconTheme::get_for_display(display);
    icon_theme = theme1->get_theme_name();

    // Check "dark" string
    size_t theme_str_length = icon_theme.length();
    tmp_str = icon_theme.substr(theme_str_length - 4, theme_str_length - 1);
    if (tmp_str.lowercase() == "dark")
    {
        return true;
    }
    else
    {
        return false;
    }
}

void MyWin::titlebar_init()
{
    // Add HeaderBar
    // header.set_title("Xe Release 15");

    // Add stack widget
    // header.set_custom_title(switcher);
    header.set_title_widget(switcher);
    header.set_show_title_buttons();
    header.set_decoration_layout("close,minimize:menu");
    set_titlebar(header);

    // Initalize Menu
    menu_builder = Gtk::Builder::create_from_resource("/org/gtk/daleclack/menubar.xml");
    auto object = menu_builder->get_object<Gio::MenuModel>("app-menu");
    popover.set_menu_model(object);

    // Add a check button for dark mode
    // popover.add_child(check_dark, "check_dark");

    // Add Menu Actions
    add_action("configs", sigc::mem_fun(*this, &MyWin::config_dialog));
    add_action("back1", sigc::mem_fun(*this, &MyWin::background1));
    add_action("back2", sigc::mem_fun(*this, &MyWin::background2));
    add_action("back3", sigc::mem_fun(*this, &MyWin::background3));
    add_action("about", sigc::mem_fun(*this, &MyWin::about_dialog));
    add_action("quit", sigc::mem_fun(*this, &MyWin::hide));

    // Initalize MenuButton
    menubtn.set_icon_name("open-menu");
    menubtn.set_popover(popover);
    header.pack_end(menubtn);
}

void MyWin::background1()
{
    // Set Background Image
    auto pixbuf = Gdk::Pixbuf::create_from_xpm_data(winpe);
    auto sized = pixbuf->scale_simple(640, 360, Gdk::InterpType::BILINEAR);
    background.set_pixbuf(sized);
    // gtk_image_set_from_pixbuf(background.gobj(), sized->gobj());
    // overlay.add(background);
    // Free Memory
    pixbuf.reset();
    sized.reset();

    // Update config
    if (!start)
    {
        prefs->background_id = 1;
        prefs->save_config_now();
    }
    else
    {
        start = false;
    }
}

void MyWin::background2()
{
    // Set Background Image
    auto pixbuf = Gdk::Pixbuf::create_from_xpm_data(img7);
    auto sized = pixbuf->scale_simple(640, 360, Gdk::InterpType::BILINEAR);
    background.set_pixbuf(sized);
    // gtk_image_set_from_pixbuf(background.gobj(), sized->gobj());
    // overlay.add(background);
    // Free Memory
    pixbuf.reset();
    sized.reset();

    // Update config
    if (!start)
    {
        prefs->background_id = 2;
        prefs->save_config_now();
    }
    else
    {
        start = false;
    }
}

void MyWin::background3()
{
    // Set Background Image
    auto pixbuf = Gdk::Pixbuf::create_from_xpm_data(fly);
    auto sized = pixbuf->scale_simple(640, 360, Gdk::InterpType::BILINEAR);
    background.set_pixbuf(sized);
    // gtk_image_set_from_pixbuf(background.gobj(), sized->gobj());
    // overlay.add(background);
    // Free Memory
    pixbuf.reset();
    sized.reset();

    // Update config
    if (!start)
    {
        prefs->background_id = 3;
        prefs->save_config_now();
    }
    else
    {
        start = false;
    }
}

void MyWin::config_dialog()
{
    load_config();
    prefs->init_json_data(data);
    stack1.set_visible_child(cfg_box);
}

void MyWin::load_config()
{
    // Load/Reload json config file

    // Open json file
    std::ifstream json_file("xe_config.json");
    if (json_file.is_open())
    {
        // Read data from json file
        data = json::parse(json_file);
        config_longterm = data["Longterm"];
        config_stable = data["Stable"];
        config_devel = data["Develop"];
        // check_dark.set_active(data["dark_mode"]);
        back_id = data["background"];
    }
    else
    {
        msg_dialog.Init("The config doesn't exist!\nPlease use \"Config\" menu to set releases");
        msg_dialog.present();
        // check_dark.set_active(false);
        back_id = 3;
        return;
    }
    json_config_init(data);
    json_file.close();
}

void MyWin::on_window_hide(Gtk::Window *window)
{
    delete window;
}

void MyWin::main_releases()
{
    // Get Selection
    // int version = combo.get_active_row_number();
    int version = drop_down.get_selected();
    char str[57];
    // Get Configs
    load_config();
    switch (version) // Use Selection to Perform
    {
    case Releases::LTS:
#ifdef DISABLE_LONGTERM_BUILD
        msg_dialog.Init("The longterm build is diasbled!");
        msg_dialog.present();
#else
        longterm(local, config_longterm.c_str(), str);
        msg_dialog.Init(str);
        msg_dialog.present();
#endif
        break;
    case Releases::Stable:
#ifdef DISABLE_STABLE_BUILD
        msg_dialog.Init("The stable build is diasbled!");
        msg_dialog.present();
#else
        stable(local, config_stable.c_str(), str);
        msg_dialog.Init(str);
        msg_dialog.present();
#endif
        break;
    case Releases::Dev:
#ifdef DISABLE_DEVELOP_BUILD
        msg_dialog.Init("The development build is diasbled!");
        msg_dialog.present();
#else
        develop(local, config_devel.c_str(), str);
        msg_dialog.Init(str);
        msg_dialog.present();
#endif
        break;
    }
}

void MyWin::check_toggled()
{
    // Get the state of check button and set the config to json file
    // prefs->set_dark_mode(check_dark.get_active());
    prefs->save_config_now();
}

void MyWin::about_dialog()
{
    char *version, *copyright;
    // The Gtkmm Version
    version = g_strdup_printf("17.0\nRunning Against Gtkmm %d.%d.%d\n2023 Update Summary edition",
                              GTKMM_MAJOR_VERSION,
                              GTKMM_MINOR_VERSION,
                              GTKMM_MICRO_VERSION);
    const char *authors[] = {"Dale Clack", NULL};
    // Copyright Informaion
    copyright = g_strdup_printf("© 2019—%d The Xe Project", local->tm_year + 1900);
    // Show the about dialog
    gtk_show_about_dialog(GTK_WINDOW(this->gobj()),
                          "program-name", "Xe Release",
                          "version", version,
                          "copyright", copyright,
                          "comments", "A program for generate version",
                          "authors", authors,
                          "license-type", GTK_LICENSE_GPL_3_0,
                          "logo-icon-name", "Xe-Release",
                          "title", "About Xe Release",
                          NULL, nullptr);
    // Free memory
    g_free(version);
    g_free(copyright);
}
