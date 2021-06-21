#include "MyWin.hh"
#include "img7.xpm"

MyWin::MyWin(){
    //Initalize window
    set_icon_name("org.gtk.daleclack");
    set_default_size(640,360); 
    //Set Background Image
    auto pixbuf=Gdk::Pixbuf::create_from_xpm_data(img7);
    auto sized=pixbuf->scale_simple(640,360,Gdk::INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(background.gobj(),sized->gobj());
    overlay.add(background);
    //Show everything
    add(overlay);
    show_all_children();
    //Free Memory
    pixbuf.reset();
    sized.reset();
}
