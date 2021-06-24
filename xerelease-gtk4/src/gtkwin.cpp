#include <cstdio>
#include <cstring>
#include "gtkwin.h"
#include "xerelease.h"
#include "MyDialog.h"

static gpointer app;

void app_init(GtkApplication *application){
    app=application;
}

void MsgBox(const gchar *msg,gpointer app){
    GtkWidget *dialog,*vbox,*hbox,*label2,*image;
    GtkWindow *win;
    //Initalize window
    dialog=gtk_dialog_new();
    gtk_window_set_default_size(GTK_WINDOW(dialog),300,150);
    gtk_window_set_icon_name(GTK_WINDOW(dialog),"org.gtk.daleclack");
    win=gtk_application_get_active_window(GTK_APPLICATION(app));
    gtk_window_set_transient_for(GTK_WINDOW(dialog),win);
    gtk_dialog_add_button(GTK_DIALOG(dialog),"OK",GTK_RESPONSE_OK);
    vbox=gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    //Create a hbox and parse message
    hbox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,10);
    //Get image
    GdkPixbuf *pixbuf=gdk_pixbuf_new_from_resource("/gtk60/icon.png",NULL);
    GdkPixbuf *sized=gdk_pixbuf_scale_simple(pixbuf,40,40,GDK_INTERP_BILINEAR);
    image=gtk_image_new_from_pixbuf(sized);
    gtk_widget_set_size_request(image,40,40);

    //Get Message
    gtk_box_append(GTK_BOX(hbox),image);
    label2=gtk_label_new(msg);
    gtk_widget_set_hexpand(label2,TRUE);
    gtk_widget_set_vexpand(label2,TRUE);
    gtk_box_append(GTK_BOX(hbox),label2);

    //Set hbox proprties
    gtk_widget_set_hexpand(hbox,TRUE);
    gtk_widget_set_vexpand(hbox,TRUE);
    gtk_box_append(GTK_BOX(vbox),hbox);
    g_signal_connect_swapped(dialog,"response",G_CALLBACK(gtk_window_destroy),dialog);
    gtk_widget_show(dialog);
    g_object_unref(pixbuf);
    g_object_unref(sized);
}

void config1_activated(GSimpleAction *action,
                       GVariant      *parameter,
                       gpointer      app)
{
    InputBox("Input Xe LTS Config","config_lts",app);
}

void config2_activated(GSimpleAction *action,
                       GVariant      *parameter,
                       gpointer      app)
{
    InputBox("Input Xe Stable Config","config_stable",app);
}

void config3_activated(GSimpleAction *action,
                       GVariant      *parameter,
                       gpointer      app)
{
    InputBox("Input Xe Devel Config","config_devel",app);
}

void InputBox(const char *content,const char *filename,gpointer app){
    GtkWindow *win;
    win=gtk_application_get_active_window(GTK_APPLICATION(app));
    MyDialog *dialog1;
    dialog1=my_dialog_new();
    gtk_window_set_transient_for(GTK_WINDOW(dialog1),win);
    my_dialog_set_filename(dialog1,filename);
    my_dialog_set_msg(dialog1,content);
    gtk_widget_show(GTK_WIDGET(dialog1));
}

void about_activated(GSimpleAction *action,
                     GVariant      *parameter,
                     gpointer      app)
{
    GtkWindow *win;
    win=gtk_application_get_active_window(GTK_APPLICATION(app));
    //Authors and version Information
    const char *authors[]={
        "Dale Clack",
        NULL
    };
    char *version;
    version=g_strdup_printf("11.1\nRunning Against GTK %d.%d.%d",
                            gtk_get_major_version(),
                            gtk_get_minor_version(),
                            gtk_get_micro_version());
    //Show Dialog
    GtkWidget *dialog;
    dialog=gtk_about_dialog_new();
    gtk_show_about_dialog(win,
    "program-name","Xe Release",
    "version",version,
    "copyright", "© 2019—2021 The Xe Project",
    "comments", "A Tool for Xe Release Version",
    "authors",authors,
    "logo-icon-name","Xe-Release",
    "title","About Xe-Release",
    NULL);
    g_free(version);
    //MsgBox("About Xe release 10(Gtk4 Version)\n2019-2021 Xe Corporation",app);
}

void quit_activated(GSimpleAction *action,
                    GVariant      *parameter,
                    gpointer      app)
{
    g_application_quit(G_APPLICATION(app));
}

void print(GtkWidget *widget,gpointer data){
    //Get local time
    time_t t;
    t=time(NULL);
    struct tm *local;
    local=localtime(&t);
    const gchar *filename;
    int flag;//flag=0,longterm;flag=1,stable;flag=3,develment
    //Use the combo box id to check the selection
    int id=gtk_combo_box_get_active(GTK_COMBO_BOX(data));
    switch(id){
        case 0:
        filename="config_lts";flag=0;break;
        case 1:
        filename="config_stable";flag=1;break;
        case 2:
        filename="config_devel";flag=2;break;
    }
    char str[57],ver[15];
    FILE *fp;
    fp=fopen(filename,"r");
    if(fp==NULL){
        MsgBox("The config file not exist!\nUse Change config menu for a config",app);
    }else{
        switch(flag){
        case 0:
            fscanf(fp,"%s",ver);
            longterm(local,ver,str);
            MsgBox(str,app);
            break;
        case 1:
            fscanf(fp,"%s",ver);
            stable(local,ver,str);
            MsgBox(str,app);
            break;
        case 2:
            fscanf(fp,"%s",ver);
            develop(local,ver,str);
            MsgBox(str,app);
        }
    }
    fclose(stdin);
}