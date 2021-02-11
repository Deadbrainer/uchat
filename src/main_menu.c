#include "../inc/uchat.h"

char *pages[] = {
	"Welcome to Gtk+3.0",
	"page2",
	"page3",
	NULL
};

GdkEvent *event_k;

int do_event(GtkWidget *widget, GdkEventKey *event)
{
    if(event->keyval == GDK_BUTTON_SECONDARY)
    {
        gtk_main_do_event(event_k);
    }
    return false;
}

int on_key_press(GtkWidget *widget, GdkEventKey *event, GtkTextView *textview)
{
    if (event->keyval == GDK_KEY_KP_Enter || event->keyval == GDK_KEY_Return)
    {
        GtkTextIter iter;
        GtkTextIter citer;
        GtkTextMark *cursor;
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));

        const gchar *text = gtk_entry_get_text(GTK_ENTRY(widget));
        //GtkTextTag *tag = gtk_text_buffer_create_tag(buffer, "edit", "editable", true, NULL);

        GtkTextTagTable *tag_table = gtk_text_buffer_get_tag_table(buffer);
        GtkTextTag *tag = gtk_text_tag_table_lookup(tag_table, "edit");

        event_k = gdk_event_new(GDK_BUTTON_SECONDARY);
        while(gtk_text_tag_event(tag, G_OBJECT(textview), event_k, &iter))
        {
            printf("%s", "test");
        }


        //int sock = 0;
        //get_sockid(&sock, 0);
        //fprintf(stderr, "%d\n", sock);
        //send();

        cursor = gtk_text_buffer_get_mark(buffer, "insert");
        gtk_text_buffer_get_iter_at_mark(buffer, &iter, cursor);
        gtk_text_iter_forward_to_end(&iter);
        gtk_text_buffer_place_cursor(buffer, &iter);


        gtk_text_buffer_insert_with_tags(buffer, &iter, text, -1, tag, NULL);
        gtk_text_buffer_insert(buffer, &iter, "\n\n", -1);

        gtk_text_buffer_insert(buffer, &iter, "\n test for no edit \n\n", -1);


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

GtkWidget *textAction(GtkWidget **stack, int pos)
{
    GtkWidget *textArea = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(textArea), false);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(textArea), false);
    g_signal_connect(G_OBJECT(textArea), "button-press-event", G_CALLBACK(do_event), NULL);

    GtkTextBuffer *buffer;
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textArea));

    GtkTextTag *tag = gtk_text_buffer_create_tag(buffer, "edit", "editable", true, NULL);

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
    gtk_stack_add_named(GTK_STACK(*stack), vbox, pages[pos]);
    gtk_container_child_set(GTK_CONTAINER(*stack), vbox, "title", pages[pos], NULL);
    return *stack;
}


void main_menu()
{
    GtkWidget *sidebar;
    sidebar = gtk_stack_sidebar_new();

    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(hbox), sidebar, false, false, 5);

    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    new_window(main_window, 1000, 800, TRUE, 10, "uchat");
    gtk_widget_add_events(main_window, GDK_KEY_PRESS_MASK); //* key scanning
    gtk_widget_add_events(main_window, GDK_BUTTON_PRESS_MASK);

    g_signal_connect(G_OBJECT(main_window), "delete-event", G_CALLBACK(closeApp), NULL);

    GtkWidget *stack = gtk_stack_new();
    gtk_stack_set_transition_type(GTK_STACK(stack), GTK_STACK_TRANSITION_TYPE_SLIDE_UP_DOWN);//The effect of switching is upward
	gtk_stack_sidebar_set_stack(GTK_STACK_SIDEBAR(sidebar), GTK_STACK(stack));
    gtk_box_pack_start(GTK_BOX(hbox), gtk_separator_new(GTK_ORIENTATION_VERTICAL), FALSE, FALSE, 0);     


    stack = textAction(&stack, 0);      
    stack = textAction(&stack, 1);                     
	gtk_box_pack_start(GTK_BOX(hbox), stack, TRUE, TRUE, 0);

    gtk_container_add(GTK_CONTAINER(main_window), hbox);
    gtk_widget_show_all(main_window);
}
