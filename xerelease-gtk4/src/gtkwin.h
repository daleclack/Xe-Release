#ifndef __GTK_WIN_H_
#define __GTK_WIN_H_

#include <gtk/gtk.h>

void app_init(GtkApplication *application);

void MsgBox(const gchar *msg,gpointer app);

void InputBox(const gchar *content,gpointer app);

void InputBox_activated(GtkWidget *widget,int response,gpointer app);

void entry_activate(GtkWidget *widget,gpointer data);

void config1_activated(GSimpleAction *action,GVariant *parameter,gpointer app);

void config2_activated(GSimpleAction *action,GVariant *parameter,gpointer app);

void config3_activated(GSimpleAction *action,GVariant *parameter,gpointer app);

void about_activated(GSimpleAction *action,GVariant *parameter,gpointer app);

void quit_activated(GSimpleAction *action,GVariant *parameter,gpointer app);

void print(GtkWidget *widget,gpointer data);

#endif