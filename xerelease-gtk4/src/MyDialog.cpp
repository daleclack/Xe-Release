#include "MyDialog.h"
#include <cstdio>

struct _MyDialog{
    GtkDialog parent;
    GtkWidget * test_entry;
    GtkWidget * msg_label;
    const char * filename;
};

G_DEFINE_TYPE(MyDialog,my_dialog,GTK_TYPE_DIALOG)

static void entry_response(GtkWidget *widget,GtkDialog *dialog){
    gtk_dialog_response(dialog,GTK_RESPONSE_OK);
}

static void my_dialog_init(MyDialog *dialog){
    GtkWidget *content_area,*label,*vbox;
    //Initalize Dialog
    gtk_window_set_icon_name(GTK_WINDOW(dialog),"org.gtk.daleclack");
    gtk_window_set_default_size(GTK_WINDOW(dialog),300,150);
    gtk_dialog_add_buttons(GTK_DIALOG(dialog),"OK",GTK_RESPONSE_OK,"Cancel",GTK_RESPONSE_CANCEL,NULL);
    //Get Content Area and add widgets
    content_area=gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    vbox=gtk_box_new(GTK_ORIENTATION_VERTICAL,5);
    gtk_widget_set_hexpand(vbox,TRUE);
    gtk_widget_set_vexpand(vbox,TRUE);
    label=gtk_label_new(" ");
    gtk_box_append(GTK_BOX(vbox),label);
    //Label to Show Message
    dialog->msg_label=gtk_label_new("");
    gtk_box_append(GTK_BOX(vbox),dialog->msg_label);
    //Entry to input Config
    dialog->test_entry=gtk_entry_new();
    gtk_widget_set_hexpand(dialog->test_entry,TRUE);
    g_signal_connect(dialog->test_entry,"activate",G_CALLBACK(entry_response),dialog);
    GtkEntryBuffer *buffer=gtk_entry_get_buffer(GTK_ENTRY(MY_DIALOG(dialog)->test_entry));
    gtk_entry_buffer_set_text(buffer,"Default Config",-1);
    gtk_box_append(GTK_BOX(vbox),dialog->test_entry);
    gtk_box_append(GTK_BOX(content_area),vbox);
}

static void dialog_response(GtkDialog *dialog,int response){
    //Get Text at response
    if(response==GTK_RESPONSE_OK){
        GtkEntryBuffer *buffer=gtk_entry_get_buffer(GTK_ENTRY(MY_DIALOG(dialog)->test_entry));
        const char *content=gtk_entry_buffer_get_text(buffer);
        const char *filename=MY_DIALOG(dialog)->filename;
        freopen(filename,"w",stdout);
        g_print("%s\n",content);
        fclose(stdout);
    }
    gtk_window_destroy(GTK_WINDOW(dialog));
    //gtk_widget_destroy(GTK_WIDGET(dialog));
}

static void my_dialog_class_init(MyDialogClass *class1){
    GTK_DIALOG_CLASS(class1)->response=dialog_response;
}

void my_dialog_set_filename(MyDialog *dialog,const char *filename){
    dialog->filename=filename;
}

void my_dialog_set_msg(MyDialog *dialog,const char *msg){
    gtk_label_set_label(GTK_LABEL(dialog->msg_label),msg);
}

MyDialog * my_dialog_new(){
    return MY_DIALOG(g_object_new(MY_DIALOG_TYPE,NULL));
}
