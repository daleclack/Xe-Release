#include "MyWin.hh"
#include "img7.xpm"
#include "winpe.xpm"
#include "fly.xpm"
#include "xeapi.hh"
#include "xerelease.hh"
#include <cstdio>

enum Releases
{
    LTS,
    Stable,
    Dev
};

MyWin::MyWin()
    : btn_box(Gtk::ORIENTATION_VERTICAL, 5),
      btn_ver("Xe-Ver"),
      msg_dialog(*this)
{
    // Initalize window
    set_icon_name("Xe-Release");
    set_default_size(640, 360);
    titlebar_init();

    // Set Background Image
    auto pixbuf = Gdk::Pixbuf::create_from_xpm_data(fly);
    auto sized = pixbuf->scale_simple(640, 360, Gdk::INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(background.gobj(), sized->gobj());
    overlay.add(background);

    // Get Local time
    time_t t;
    t = time(NULL);
    local = localtime(&t);

    // Initalize Api Label
    sprintf(api_version, "Xe Api Version:%d", xeapi1(local));
    api_label.set_label(api_version);

    // Initalize combobox
    combo.append("Longterm");
    combo.append("Stable");
    combo.append("Development");
    combo.set_active(1);

    // Add Main Controls
    btn_box.set_halign(Gtk::ALIGN_CENTER);
    btn_box.set_valign(Gtk::ALIGN_CENTER);
    btn_box.pack_start(api_label, Gtk::PACK_SHRINK);
    btn_box.pack_start(combo, Gtk::PACK_SHRINK);
    btn_box.pack_start(btn_ver, Gtk::PACK_SHRINK);
    btn_box.set_opacity(0.7);
    overlay.add_overlay(btn_box);
    btn_ver.signal_clicked().connect(sigc::mem_fun(*this, &MyWin::main_releases));

    // Show everything
    add(stack1);
    stack1.add(overlay, "main_page", "WelCome");
    switcher.set_stack(stack1);
    show_all_children();

    // Free Memory
    pixbuf.reset();
    sized.reset();
}

void MyWin::titlebar_init()
{
    // Add HeaderBar
    // header.set_title("Xe Release 15");

    // Add stack widget
    header.set_custom_title(switcher);
    header.set_show_close_button();
    header.set_decoration_layout("close,minimize:menu");
    set_titlebar(header);

    // Initalize Menu
    menu_builder = Gtk::Builder::create_from_resource("/org/gtk/daleclack/menubar.xml");
    auto object = menu_builder->get_object("app-menu");
    auto gmenu = Glib::RefPtr<Gio::Menu>::cast_dynamic(object);
    popover.bind_model(gmenu);

    // Add Menu Actions
    add_action("configs", sigc::mem_fun(*this, &MyWin::config_dialog));
    add_action("back1", sigc::mem_fun(*this, &MyWin::background1));
    add_action("back2", sigc::mem_fun(*this, &MyWin::background2));
    add_action("back3", sigc::mem_fun(*this, &MyWin::background3));
    add_action("about", sigc::mem_fun(*this, &MyWin::about_dialog));
    add_action("quit", sigc::mem_fun(*this, &MyWin::hide));

    // Initalize MenuButton
    menubtn.set_image_from_icon_name("open-menu");
    menubtn.set_popover(popover);
    header.pack_end(menubtn);
}

void MyWin::background1()
{
    // Set Background Image
    auto pixbuf = Gdk::Pixbuf::create_from_xpm_data(winpe);
    auto sized = pixbuf->scale_simple(640, 360, Gdk::INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(background.gobj(), sized->gobj());
    // overlay.add(background);
    // Free Memory
    pixbuf.reset();
    sized.reset();
}

void MyWin::background2()
{
    // Set Background Image
    auto pixbuf = Gdk::Pixbuf::create_from_xpm_data(img7);
    auto sized = pixbuf->scale_simple(640, 360, Gdk::INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(background.gobj(), sized->gobj());
    // overlay.add(background);
    // Free Memory
    pixbuf.reset();
    sized.reset();
}

void MyWin::background3()
{
    // Set Background Image
    auto pixbuf = Gdk::Pixbuf::create_from_xpm_data(fly);
    auto sized = pixbuf->scale_simple(640, 360, Gdk::INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(background.gobj(), sized->gobj());
    // overlay.add(background);
    // Free Memory
    pixbuf.reset();
    sized.reset();
}

void MyWin::config_dialog()
{
    auto dialog = MyDialog::create(*this);
    dialog->signal_hide().connect(sigc::bind(sigc::mem_fun(*this, &MyWin::on_window_hide), dialog));
    dialog->present();
}

void MyWin::on_window_hide(Gtk::Window *window)
{
    delete window;
}

void MyWin::main_releases()
{
    // Get Selection
    int version = combo.get_active_row_number();
    char str[57];
    // Get Configs
    std::string config_longterm, config_stable, config_devel;
    // Open json file
    std::ifstream json_file("xe_config.json");
    json data;
    if (json_file.is_open())
    {
        // Read data from json file
        data = json::parse(json_file);
        config_longterm = data["Longterm"];
        config_stable = data["Stable"];
        config_devel = data["Develop"];
    }
    else
    {
        msg_dialog.Init("The config doesn't exist!\nPlease use \"Config\" menu to set releases");
        msg_dialog.show_all();
        return;
    }
    json_file.close();

    switch (version) // Use Selection to Perform
    {
    case Releases::LTS:
        longterm(local, config_longterm.c_str(), str);
        msg_dialog.Init(str);
        msg_dialog.show_all();
        break;
    case Releases::Stable:
        stable(local, config_stable.c_str(), str);
        msg_dialog.Init(str);
        msg_dialog.show_all();
        break;
    case Releases::Dev:
        develop(local, config_devel.c_str(), str);
        msg_dialog.Init(str);
        msg_dialog.show_all();
        break;
    }
}

void MyWin::about_dialog()
{
    char *version, *copyright;
    // The Gtkmm Version
    version = g_strdup_printf("15.0\nRunning Against Gtkmm %d.%d.%d",
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
                          NULL);
    // Free memory
    g_free(version);
    g_free(copyright);
}
