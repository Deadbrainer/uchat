#include "../inc/uchat.h"

void closeApp(GtkWidget *window, gpointer data)
{
    gtk_main_quit();
}

void window_switch(GtkWidget *window, gpointer data)
{
    if(data == reg_window) 
    {
        login_menu();
        gtk_widget_destroy(reg_window);
    }
    else if(data == log_window)
    {
        reg_menu();
        gtk_widget_destroy(log_window);
    }
    else gtk_main_quit();
}
