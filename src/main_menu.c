#include "../inc/uchat.h"

void main_menu()
{
    GtkWidget *text, *text_entry;
    GtkWidget *vbox;

    GtkEntryBuffer *text_buff;;

    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    new_window(main_window, 1000, 800, TRUE, 10, "uchat");
    g_signal_connect(G_OBJECT(log_window), "delete-event", G_CALLBACK(closeApp), NULL);

    text = gtk_text_view_new();
    text_buff = gtk_entry_buffer_new("Fuck u", 7);
    text_entry = gtk_entry_new_with_buffer(text_buff);
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    gtk_box_pack_start(GTK_BOX(vbox), text, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), text_entry, FALSE, FALSE, 5);
    

    gtk_container_add(GTK_CONTAINER(main_window), vbox);
    gtk_widget_show_all(main_window);
}
