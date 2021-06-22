#include "MyWin.hh"
#include "img7.xpm"
#include "winpe.xpm"

MyWin::MyWin()
:btn_box(Gtk::ORIENTATION_VERTICAL,5),
btn_ver("Xe-Ver")
{
    //Initalize window
    set_icon_name("Xe-Release");
    set_default_size(640,360);
    titlebar_init();

    //Set Background Image
    auto pixbuf=Gdk::Pixbuf::create_from_xpm_data(img7);
    auto sized=pixbuf->scale_simple(640,360,Gdk::INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(background.gobj(),sized->gobj());
    overlay.add(background);

    //Initalize combobox
    combo.append("Longterm");
    combo.append("Stable");
    combo.append("Development");
    combo.set_active(1);

    //Add Main Controls
    btn_box.set_halign(Gtk::ALIGN_CENTER);
    btn_box.set_valign(Gtk::ALIGN_CENTER);
    btn_box.pack_start(combo,Gtk::PACK_SHRINK);
    btn_box.pack_start(btn_ver,Gtk::PACK_SHRINK);
    overlay.add_overlay(btn_box);

    //Show everything
    add(overlay);
    show_all_children();

    //Free Memory
    pixbuf.reset();
    sized.reset();
}

void MyWin::titlebar_init(){
    //Add HeaderBar
    header.set_title("Xe Release 12");
    header.set_show_close_button();
    header.set_decoration_layout("close,minimize:menu");
    set_titlebar(header);
    
    //Initalize Menu
    menu_builder=Gtk::Builder::create_from_resource("/XeRelease/menubar.xml");
    auto object=menu_builder->get_object("app-menu");
    auto gmenu=Glib::RefPtr<Gio::Menu>::cast_dynamic(object);
    popover.bind_model(gmenu);

    //Add Menu Actions
    add_action("back1",sigc::mem_fun(*this,&MyWin::background1));
    add_action("back2",sigc::mem_fun(*this,&MyWin::background2));
    add_action("quit",sigc::mem_fun(*this,&MyWin::hide));

    //Initalize MenuButton
    menubtn.set_image_from_icon_name("open-menu");
    menubtn.set_popover(popover);
    header.pack_end(menubtn);
}

void MyWin::background1(){
    //Set Background Image
    auto pixbuf=Gdk::Pixbuf::create_from_xpm_data(winpe);
    auto sized=pixbuf->scale_simple(640,360,Gdk::INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(background.gobj(),sized->gobj());
    //overlay.add(background);
    //Free Memory
    pixbuf.reset();
    sized.reset();
}

void MyWin::background2(){
    //Set Background Image
    auto pixbuf=Gdk::Pixbuf::create_from_xpm_data(img7);
    auto sized=pixbuf->scale_simple(640,360,Gdk::INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(background.gobj(),sized->gobj());
    //overlay.add(background);
    //Free Memory
    pixbuf.reset();
    sized.reset();
}
