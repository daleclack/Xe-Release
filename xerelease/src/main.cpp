#include <gtk/gtk.h>
#include "gtkwin.h"
#include "src/img7.xpm"

static GActionEntry app_entry[] =
{
    {"quit",quit_activated,NULL,NULL,NULL},
    {"about",about_activated,NULL,NULL,NULL},
    {"config1",config1_activated,NULL,NULL,NULL},
    {"config2",config2_activated,NULL,NULL,NULL},
    {"config3",config3_activated,NULL,NULL,NULL}
};

static void gtkmain(GtkApplication *app,gpointer user_data){
    GtkWidget *window,*header,*menubtn,*popover,*overlay,*background;
    GtkBuilder *builder=gtk_builder_new_from_resource("/gtk59/menubar.xml");
    GMenuModel *model;
    //Initalize window
    window=gtk_application_window_new(app);
    gtk_window_set_icon_name(GTK_WINDOW(window),"org.gtk.daleclack");
    gtk_window_set_default_size(GTK_WINDOW(window),448,252);
    //gtk_window_set_title(GTK_WINDOW(window),"gtk (57)");
    g_action_map_add_action_entries(G_ACTION_MAP(app),app_entry,
                                    G_N_ELEMENTS (app_entry),app);
    app_init(app);
    //Header bar
    header=gtk_header_bar_new();
    gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(header),TRUE);
    gtk_header_bar_set_title(GTK_HEADER_BAR(header),"Xe release 11");
    gtk_header_bar_set_decoration_layout(GTK_HEADER_BAR(header),"close,minimize:menu");
    //A application menu
    menubtn=gtk_menu_button_new();
    model=G_MENU_MODEL(gtk_builder_get_object(builder,"app-menu"));
    popover=gtk_popover_new_from_model(menubtn,model);
    gtk_widget_set_halign(popover,GTK_ALIGN_END);
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header),menubtn);
    gtk_menu_button_set_popover(GTK_MENU_BUTTON(menubtn),popover);

    //background for window
    overlay=gtk_overlay_new();
    GdkPixbuf *pixbuf=gdk_pixbuf_new_from_xpm_data(img7);
    GdkPixbuf *sized=gdk_pixbuf_scale_simple(pixbuf,448,252,GDK_INTERP_BILINEAR);
    background=gtk_image_new_from_pixbuf(sized);
    gtk_container_add(GTK_CONTAINER(overlay),background);
    //gtk_overlay_set_child(GTK_OVERLAY(overlay),background);
    //gtk_overlay_set_measure_overlay(GTK_OVERLAY(overlay),background,TRUE);

    //Main widgets
    GtkWidget *vbox,*combo,*btn_ver;
    vbox=gtk_box_new(GTK_ORIENTATION_VERTICAL,10);
    //Combo box to select version
    combo=gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo),"Longterm");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo),"Stable");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo),"Development");
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo),1);
    btn_ver=gtk_button_new_with_label("Xe-Ver");
    g_signal_connect(btn_ver,"clicked",G_CALLBACK(print),combo);
    gtk_box_pack_start(GTK_BOX(vbox),combo,FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(vbox),btn_ver,FALSE,FALSE,0);
    gtk_widget_set_halign(vbox,GTK_ALIGN_CENTER);
    gtk_widget_set_valign(vbox,GTK_ALIGN_CENTER);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay),vbox);

    gtk_window_set_titlebar(GTK_WINDOW(window),header);
    gtk_container_add(GTK_CONTAINER(window),overlay);
    gtk_widget_show_all(window);
    g_object_unref(pixbuf);
    g_object_unref(sized);
}

int main(int argc,char **argv){
    GtkApplication *app;
    app=gtk_application_new("org.gtk.daleclack",G_APPLICATION_NON_UNIQUE);
    g_signal_connect(app,"activate",G_CALLBACK(gtkmain),NULL);
    return g_application_run(G_APPLICATION(app),argc,argv);
}