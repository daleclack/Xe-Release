#include "MyWin.hh"
#include "img7.xpm"
#include "winpe.xpm"
#include "xeapi.hh"
#include "xerelease.hh"
#include <cstdio>

enum Releases{
    LongTerm,
    Stable,
    Develop,
};

MyWin::MyWin()
:btn_box(Gtk::Orientation::VERTICAL,5),
btn_ver("Xe-Ver"),
msg_dialog(*this),
input_dialog(*this)
{
    //Initalize window
    set_icon_name("Xe-Release");
    set_default_size(640,360+47);
    set_title("Xe Release 12");
    titlebar_init();
 
    //Set Background Image
    auto pixbuf=Gdk::Pixbuf::create_from_xpm_data(img7);
    auto sized=pixbuf->scale_simple(640,360,Gdk::InterpType::BILINEAR);
    background.set_pixbuf(sized);
    background.set_margin(0);
    overlay.set_child(background);
    overlay.set_margin(0);

    //Get Local time
    time_t t;
    t=time(NULL);
    local=localtime(&t);

    //Initalize Api Label
    sprintf(api_version,"Xe Api Version:%d",xeapi1(local));
    api_label.set_label(api_version);

    //Initalize combobox
    combo.append("Longterm");
    combo.append("Stable");
    combo.append("Development");
    combo.set_active(1);

    //Add Main Controls
    btn_box.set_halign(Gtk::Align::CENTER);
    btn_box.set_valign(Gtk::Align::CENTER);
    btn_box.append(api_label);
    btn_box.append(combo);
    btn_box.append(btn_ver);
    overlay.add_overlay(btn_box);
    btn_ver.signal_clicked().connect(sigc::mem_fun(*this,&MyWin::main_releases));

    //Show everythings
    set_child(overlay);
    //show();

    //Free Memory
    pixbuf.reset();
    sized.reset();
}

void MyWin::titlebar_init(){
    //Add HeaderBar
    //header.set_title("Xe Release 12");
    header.set_show_title_buttons();
    header.set_decoration_layout("close,minimize:menu");
    set_titlebar(header);
    
    //Initalize Menu
    menu_builder=Gtk::Builder::create_from_resource("/XeRelease/menubar.xml");
    auto object=menu_builder->get_object<Gio::Menu>("app-menu");
    popover.set_menu_model(object);
    popover.set_halign(Gtk::Align::END);

    //Add Menu Actions
    add_action("config1",sigc::mem_fun(*this,&MyWin::config_lts));
    add_action("config2",sigc::mem_fun(*this,&MyWin::config_stable));
    add_action("config3",sigc::mem_fun(*this,&MyWin::config_devel));
    add_action("back1",sigc::mem_fun(*this,&MyWin::background1));
    add_action("back2",sigc::mem_fun(*this,&MyWin::background2));
    add_action("about",sigc::mem_fun(*this,&MyWin::about_dialog));
    add_action("quit",sigc::mem_fun(*this,&MyWin::hide));

    //Initalize MenuButton
    menubtn.set_popover(popover);
    menubtn.set_icon_name("open-menu");
    header.pack_end(menubtn);
}

void MyWin::background1(){
    //Set Background Image
    auto pixbuf=Gdk::Pixbuf::create_from_xpm_data(winpe);
    auto sized=pixbuf->scale_simple(640,360,Gdk::InterpType::BILINEAR);
    background.set_pixbuf(sized);
    //Free Memory
    pixbuf.reset();
    sized.reset();
}

void MyWin::background2(){
    //Set Background Image
    auto pixbuf=Gdk::Pixbuf::create_from_xpm_data(img7);
    auto sized=pixbuf->scale_simple(640,360,Gdk::InterpType::BILINEAR);
    background.set_pixbuf(sized);
    //Free Memory
    pixbuf.reset();
    sized.reset();
}

bool MyWin::get_config(const char *filename){
    fp=fopen(filename,"rt+");
    if(fp==NULL){
        msg_dialog.Init("The config file not exist!\nUse Change config menu for a config");
        msg_dialog.show();
        return false;
    }
    return true;
}

void MyWin::config_lts(){
    input_dialog.set_msg("Input Xe LTS Config");
    input_dialog.set_filename("config_lts");
    input_dialog.show();
}

void MyWin::config_stable(){
    input_dialog.set_msg("Input Xe Stable Config");
    input_dialog.set_filename("config_stable");
    input_dialog.show();
}

void MyWin::config_devel(){
    input_dialog.set_msg("Input Xe Devel Config");
    input_dialog.set_filename("config_devel");
    input_dialog.show();
}

void MyWin::main_releases(){
    //Get Selection
    int version=combo.get_active_row_number();
    char str[57],ver[15];//Version and Full Version
    switch (version)//Use Selection to Perform
    {
    case Releases::LongTerm:
        if(get_config("config_lts")){
            fscanf(fp,"%s",ver);
            longterm(local,ver,str);
            msg_dialog.Init(str);
            msg_dialog.show();
            fclose(fp);
        }
        break;
    case Releases::Stable:
        if(get_config("config_stable")){
            fscanf(fp,"%s",ver);
            stable(local,ver,str);
            msg_dialog.Init(str);
            msg_dialog.show();
            fclose(fp);
        }
        break;
    case Releases::Develop:
        if(get_config("config_devel")){
            fscanf(fp,"%s",ver);
            develop(local,ver,str);
            msg_dialog.Init(str);
            msg_dialog.show();
            fclose(fp);
        }
        break;
    }
    
}

void MyWin::about_dialog(){
    char *version;
    version=g_strdup_printf("12.1\nRunning Against Gtkmm %d.%d.%d",
                            GTKMM_MAJOR_VERSION,
                            GTKMM_MINOR_VERSION,
                            GTKMM_MICRO_VERSION);
    const char *authors[]={"Dale Clack",NULL};
    gtk_show_about_dialog(GTK_WINDOW(this->gobj()),
                          "program-name","Xe Release",
                          "version",version,
                          "copyright", "© 2019—2021 The Xe Project",
                          "comments", "A program for generate version",
                          "authors",authors,
                          "license-type",GTK_LICENSE_GPL_3_0,
                          "logo-icon-name","Xe-Release",
                          "title","About Xe Release",
                          NULL);
}
