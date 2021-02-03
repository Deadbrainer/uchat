#include "../inc/uchat.h"

GtkWidget *textArea;

void text_add()
{
    
}

int on_key_press(GtkWidget *widget, GdkEventKey *event, GtkTextBuffer *buffer/*gpointer user_data*/)
{
    if(event->keyval == GDK_KEY_KP_Enter || event->keyval == GDK_KEY_Return)
    {
        GtkTextIter iter;
        GtkTextIter end;
        GtkTextMark *cursor;
        cursor = gtk_text_buffer_get_mark (buffer, "insert");
        gtk_text_buffer_get_iter_at_mark (buffer, &iter, cursor);
        gtk_text_iter_set_line_offset (&iter, 0);
        gtk_text_buffer_insert (buffer, &iter, "<li>", -1);
        gtk_text_iter_forward_to_line_end (&iter);
        gtk_text_buffer_insert (buffer, &iter, "</li>", -1);
    }
    return false;
}

void main_menu()
{
    GtkWidget *scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
    GtkWidget *textEntry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(textEntry), "void");
    textArea = gtk_text_view_new();

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkTextBuffer *buffer;
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (textArea));
    gtk_text_buffer_set_text (buffer, "Item1\nItem2\nItem3", -1);


    //textview parametres
    gtk_text_view_set_editable(GTK_TEXT_VIEW(textArea), false);
    //gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(textArea), false);

    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    new_window(main_window, 1000, 800, TRUE, 10, "uchat");
    gtk_widget_add_events(main_window, GDK_KEY_PRESS_MASK); //* key scanning

    g_signal_connect(G_OBJECT(main_window), "delete-event", G_CALLBACK(closeApp), NULL);
    //g_signal_connect_swapped(G_OBJECT(textEntry), "key-press-event", G_CALLBACK(on_key_press), textEntry);
    g_signal_connect(G_OBJECT(main_window), "key-press-event", G_CALLBACK(on_key_press), buffer);

    gtk_container_add(GTK_CONTAINER(scrolledwindow), textArea);
    gtk_box_pack_start(GTK_BOX(vbox), scrolledwindow, true, true, 5);
    gtk_box_pack_end(GTK_BOX(vbox), textEntry, FALSE, FALSE, 5);

    //gtk_label_set_selectable(block, TRUE)

    gtk_container_add(GTK_CONTAINER(main_window), vbox);
    gtk_widget_show_all(main_window);
}
