#include "../inc/client.h"

void on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
    switch (event->keyval)
    {
    case GDK_KEY_PRESS:
        printf("yeey");
        break;
    }
}

void main_menu()
{
    GtkWidget *scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
    GtkWidget *textEntry = gtk_entry_new();
    GtkWidget *vbox;

    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    new_window(main_window, 1000, 800, TRUE, 10, "uchat");
    g_signal_connect(G_OBJECT(main_window), "delete-event", G_CALLBACK(closeApp), NULL);
    g_signal_connect(G_OBJECT(main_window), "key-press-event", G_CALLBACK(on_key_press), NULL);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    //gtk_container_add(GTK_CONTAINER(scrolledwindow), textArea);
    //gtk_box_pack_start(GTK_BOX(vbox), scrolledwindow, true, true, 5);
    gtk_box_pack_end(GTK_BOX(vbox), textEntry, FALSE, FALSE, 5);

    //gtk_label_set_selectable(block, TRUE)

    gtk_container_add(GTK_CONTAINER(main_window), vbox);
    gtk_widget_show_all(main_window);
}
