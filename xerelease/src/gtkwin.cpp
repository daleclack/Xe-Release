#include <cstdio>
#include <cstring>
#include "gtkwin.h"
#include "xerelease.h"

static const char *str,*filename;
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

    //label1=gtk_label_new("    ");
    //gtk_box_pack_start(GTK_BOX(vbox),label1,TRUE,TRUE,0);

    //Create a hbox and parse message
    hbox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,10);
    //Get image
    GdkPixbuf *pixbuf=gdk_pixbuf_new_from_resource("/gtk59/icon.png",NULL);
    GdkPixbuf *sized=gdk_pixbuf_scale_simple(pixbuf,40,40,GDK_INTERP_BILINEAR);
    image=gtk_image_new_from_pixbuf(sized);
    gtk_box_pack_start(GTK_BOX(hbox),image,FALSE,FALSE,0);
    label2=gtk_label_new(msg);
    gtk_box_pack_start(GTK_BOX(hbox),label2,TRUE,TRUE,0);
    gtk_box_pack_start(GTK_BOX(vbox),hbox,TRUE,TRUE,0);
    g_signal_connect_swapped(dialog,"response",G_CALLBACK(gtk_widget_destroy),dialog);
    gtk_widget_show_all(dialog);
    g_object_unref(pixbuf);
    g_object_unref(sized);
}

void config1_activated(GSimpleAction *action,
                              GVariant      *parameter,
                              gpointer      app)
{
    InputBox("Input config1",app);
    filename="config_lts";
}

void config2_activated(GSimpleAction *action,
                              GVariant      *parameter,
                              gpointer      app)
{
    InputBox("Input config2",app);
    filename="config_stable";
}

void config3_activated(GSimpleAction *action,
                              GVariant      *parameter,
                              gpointer      app)
{
    InputBox("Input config3",app);
    filename="config_devel";
}

void about_activated(GSimpleAction *action,
                           GVariant      *parameter,
                           gpointer      app)
{
    MsgBox("About Xe release 10(Gtk3 Version)\n2019-2021 Xe Corporation",app);
}

void quit_activated(GSimpleAction *action,
                           GVariant      *parameter,
                           gpointer      app)
{
    g_application_quit(G_APPLICATION(app));
}

void InputBox(const gchar *content,gpointer app){
    //Initalize InputBox
    GtkWindow *win;
    GtkWidget *dialog=gtk_dialog_new();
    GtkWindow *_dialog=GTK_WINDOW(dialog);
    GtkDialog *dialog1=GTK_DIALOG(dialog);
    win=gtk_application_get_active_window(GTK_APPLICATION(app));
    gtk_window_set_default_size(_dialog,300,150);
    //gtk_window_set_position(_dialog,GTK_WIN_POS_CENTER);
    gtk_window_set_title(_dialog,"Change config");
    gtk_window_set_transient_for(_dialog,win);
    gtk_dialog_add_buttons(dialog1,"Cancel",GTK_RESPONSE_CANCEL,"OK",GTK_RESPONSE_OK,NULL);

    //Get content_area and put entry and label to the dialog
    GtkWidget *content_area=gtk_dialog_get_content_area(GTK_DIALOG(_dialog));
    GtkContainer *_content_area=GTK_CONTAINER(content_area);
    GtkWidget *label=gtk_label_new(" ");
    gtk_container_add(_content_area,label);
    label=gtk_label_new(content);
    gtk_container_add(_content_area,label);
    label=gtk_label_new(" ");
    GtkWidget *entry=gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry),"default config");
    g_signal_connect(entry,"activate",G_CALLBACK(entry_activate),(gpointer)dialog);

    //MyEntry class to handle entry content and filename
    str=gtk_entry_get_text(GTK_ENTRY(entry));

    gtk_container_add(_content_area,label);
    gtk_container_add(_content_area,entry);
    gtk_widget_show_all(content_area);
    gtk_widget_show_all(dialog);
    g_signal_connect(dialog,"response",G_CALLBACK(InputBox_activated),app);
}

void InputBox_activated(GtkWidget *dialog,int response,gpointer app){
    //Check the response and update the file with the "OK" Response
    if(response==GTK_RESPONSE_OK){
        if(strlen(str)!=0){
            freopen(filename,"w",stdout);
            g_print("%s",str);
            fclose(stdout);
            gtk_widget_destroy(dialog);
            MsgBox("Config changed!\nPlease Restart the application",app);
        }else{
            gtk_widget_destroy(dialog);
        }
    }else{
    gtk_widget_destroy(dialog);
    }
    //free(filename);
}

void entry_activate(GtkWidget *widget,gpointer data){
    gtk_dialog_response(GTK_DIALOG(data),GTK_RESPONSE_OK);
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
            fgets(ver,15,fp);
            longterm(local,ver,str);
            MsgBox(str,app);
            break;
        case 1:
            fgets(ver,15,fp);
            stable(local,ver,str);
            MsgBox(str,app);
            break;
        case 2:
            fgets(ver,15,fp);
            develop(local,ver,str);
            MsgBox(str,app);
        }
    }
    fclose(stdin);
}