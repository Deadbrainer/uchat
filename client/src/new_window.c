#include "../inc/client.h"

void new_window(GtkWidget *win, int x, int z, bool resize, int border, char title[]) {
    gtk_window_set_title(GTK_WINDOW(win), title);
    gtk_window_set_position(GTK_WINDOW(win), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(win), x, z);
    gtk_window_set_resizable(GTK_WINDOW(win), FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(win), border);
}
