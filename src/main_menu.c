#include "../inc/uchat.h"

GtkWidget *g_stack;

int on_button_press(GtkWidget *textview, GdkEventButton *event, gpointer *data)
{
    if(event->type == GDK_2BUTTON_PRESS)
    {
        GtkTextIter liter;
        GtkTextIter riter;
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
        GtkTextMark *cursor = gtk_text_buffer_get_mark(buffer, "insert");
        gtk_text_buffer_get_iter_at_mark(buffer, &liter, cursor);
        gtk_text_buffer_get_iter_at_mark(buffer, &riter, cursor);
        gtk_text_iter_backward_line(&liter);
        gtk_text_iter_forward_line(&liter);
        gtk_text_iter_forward_line(&riter);
        gtk_text_iter_backward_char(&riter);

        gchar *text = gtk_text_buffer_get_text(buffer, &liter, &riter, false);
        gtk_text_buffer_insert(buffer, &riter, "Test for replacement", strlen("Test for replacement"));

        gtk_text_buffer_get_iter_at_mark(buffer, &liter, cursor);
        gtk_text_buffer_get_iter_at_mark(buffer, &riter, cursor);
        gtk_text_iter_backward_line(&liter);
        gtk_text_iter_forward_line(&liter);
        gtk_text_iter_forward_line(&riter);
        gtk_text_iter_backward_char(&riter);
        gtk_text_iter_backward_chars(&riter, strlen("Test for replacement"));

        gtk_text_buffer_delete(buffer, &liter, &riter);
    }
    return false;
}

int on_key_press(GtkWidget *widget, GdkEventKey *event, GtkTextView *textview)
{
    GtkTextIter iter;
    GtkTextIter citer;
    GtkTextMark *cursor;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
    GtkTextTagTable *tag_table = gtk_text_buffer_get_tag_table(buffer);
    GtkTextTag *font_scale = gtk_text_tag_table_lookup(tag_table, "font_size");
    GtkTextTag *left_margin = gtk_text_tag_table_lookup(tag_table, "l_margin");


    if (event->keyval == GDK_KEY_KP_Enter || event->keyval == GDK_KEY_Return)
    {
        const gchar *text = gtk_entry_get_text(GTK_ENTRY(widget));
        cursor = gtk_text_buffer_get_mark(buffer, "insert");
        gtk_text_buffer_get_iter_at_mark(buffer, &iter, cursor);
        gtk_text_iter_forward_to_end(&iter);
        gtk_text_buffer_place_cursor(buffer, &iter);

        gtk_text_buffer_insert_with_tags(buffer, &iter, text, -1, font_scale, left_margin, NULL);
        gtk_text_buffer_insert_with_tags(buffer, &iter, "\n", -1, NULL, NULL);
        gtk_entry_set_text(GTK_ENTRY(widget), "");
    }
    return false;
}

void button_pressed()
{
    addUserWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(addUserWindow), "Add Friend");
    gtk_window_set_position(GTK_WINDOW(addUserWindow), GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_window_set_default_size(GTK_WINDOW(addUserWindow), 200, 200);
    gtk_window_set_resizable(GTK_WINDOW(addUserWindow), FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(addUserWindow), 5);
    g_signal_connect(G_OBJECT(addUserWindow), "destroy", G_CALLBACK(gtk_widget_destroy), NULL);

    gtk_widget_show_all(addUserWindow);
}

GtkWidget *textAction(GtkWidget **stack, char *text)
{
    GtkWidget *textArea = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(textArea), false);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(textArea), true);
    gtk_widget_add_events(textArea, GDK_2BUTTON_PRESS);
    g_signal_connect(G_OBJECT(textArea), "button-press-event", G_CALLBACK(on_button_press), NULL);

    GtkTextBuffer *buffer;
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textArea));

    GtkTextTag *tag = gtk_text_buffer_create_tag(buffer, "font_size", "pixels_above_lines", 10, NULL);
    GtkTextTag *tag1 = gtk_text_buffer_create_tag(buffer, "l_margin", "left_margin", 5, NULL);

    GtkWidget *scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrolledwindow), textArea);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), scrolledwindow, true, true, 5);

    GtkWidget *button = gtk_button_new_with_label("Add Friend");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(button_pressed), NULL);

    GtkWidget *hbox2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    GtkWidget *textEntry = gtk_entry_new();
    g_signal_connect(G_OBJECT(textEntry), "key-press-event", G_CALLBACK(on_key_press), textArea);
    
    gtk_box_pack_start(GTK_BOX(hbox2), textEntry, true, true, 5);
    gtk_box_pack_start(GTK_BOX(hbox2), button, false, false, 0);
    gtk_box_pack_end(GTK_BOX(vbox), hbox2, FALSE, FALSE, 5);
    gtk_stack_add_named(GTK_STACK(*stack), vbox, text);
    gtk_container_child_set(GTK_CONTAINER(*stack), vbox, "title", text, NULL);
    return *stack;
}


void main_menu()
{
    GtkWidget *sidebar;
    sidebar = gtk_stack_sidebar_new();

    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(hbox), sidebar, false, false, 5);

    GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    new_window(main_window, 1000, 800, TRUE, 10, "uchat");
    gtk_widget_add_events(main_window, GDK_KEY_PRESS_MASK); //* key scanning

    g_signal_connect(G_OBJECT(main_window), "delete-event", G_CALLBACK(closeApp), NULL);

    g_stack = gtk_stack_new();
    gtk_stack_set_transition_type(GTK_STACK(g_stack), GTK_STACK_TRANSITION_TYPE_SLIDE_UP_DOWN);//The effect of switching is upward
	gtk_stack_sidebar_set_stack(GTK_STACK_SIDEBAR(sidebar), GTK_STACK(g_stack));
    gtk_box_pack_start(GTK_BOX(hbox), gtk_separator_new(GTK_ORIENTATION_VERTICAL), FALSE, FALSE, 0);     

    g_stack = textAction(&g_stack, "Games");      
    g_stack = textAction(&g_stack, "Job");                     
	gtk_box_pack_start(GTK_BOX(hbox), g_stack, TRUE, TRUE, 0);

    gtk_container_add(GTK_CONTAINER(main_window), hbox);
    gtk_widget_show_all(main_window);

}
