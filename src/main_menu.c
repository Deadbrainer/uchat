#include "../inc/uchat.h"

GtkWidget *sidebar;
char *pages[] = {
	"Welcome to Gtk+3.0",
	"page2",
	"page3",
	NULL
};

void get_text_entry(GtkWidget **textEntry, bool check)
{
    static GtkWidget *t;
    if (check)
    {
        t = *textEntry;
    }
    else
    {
        *textEntry = t;
    }
}

int on_key_press(GtkWidget *widget, GdkEventKey *event, GtkTextBuffer *buffer /*gpointer user_data*/)
{
    if (event->keyval == GDK_KEY_KP_Enter || event->keyval == GDK_KEY_Return)
    {
        GtkTextIter iter;
        GtkTextMark *cursor;
        GtkWidget *textEntry;
        get_text_entry(&textEntry, 0);

        const gchar *text = gtk_entry_get_text(GTK_ENTRY(textEntry));

        //int sock = 0;
        //get_sockid(&sock, 0);
        //fprintf(stderr, "%d\n", sock);
        //send();

        cursor = gtk_text_buffer_get_mark(buffer, "insert");
        gtk_text_buffer_get_iter_at_mark(buffer, &iter, cursor);
        gtk_text_iter_forward_to_end(&iter);
        gtk_text_buffer_place_cursor(buffer, &iter);
        gtk_text_buffer_insert(buffer, &iter, text, -1);
        gtk_text_buffer_insert(buffer, &iter, "\n\n", -1);
        gtk_entry_set_text(GTK_ENTRY(textEntry), "");
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

GtkWidget *textAction(GtkWidget **stack)
{
    GtkWidget *textArea = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(textArea), false);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(textArea), false);

    GtkTextBuffer *buffer;
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textArea));

    GtkWidget *scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrolledwindow), textArea);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), scrolledwindow, true, true, 5);

    GtkWidget *button = gtk_button_new_with_label("+");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(button_pressed), NULL);

    GtkWidget *hbox2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    GtkWidget *textEntry = gtk_entry_new();
    get_text_entry(&textEntry, 1);
    g_signal_connect(G_OBJECT(textEntry), "key-press-event", G_CALLBACK(on_key_press), buffer);
    
    gtk_box_pack_start(GTK_BOX(hbox2), textEntry, true, true, 5);
    gtk_box_pack_start(GTK_BOX(hbox2), button, false, false, 0);
    gtk_box_pack_end(GTK_BOX(vbox), hbox2, FALSE, FALSE, 5);
    gtk_stack_add_named(GTK_STACK(*stack), vbox, pages[0]);
    gtk_container_child_set(GTK_CONTAINER(*stack), vbox, "title", pages[0], NULL);
    return *stack;
}


void main_menu()
{
    //!GtkWidget *textArea = gtk_text_view_new();
 
    sidebar = gtk_stack_sidebar_new();

    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(hbox), sidebar, false, false, 5);

    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    new_window(main_window, 1000, 800, TRUE, 10, "uchat");
    gtk_widget_add_events(main_window, GDK_KEY_PRESS_MASK); //* key scanning
    g_signal_connect(G_OBJECT(main_window), "delete-event", G_CALLBACK(closeApp), NULL);

    //!GtkWidget *stack = gtk_stack_new();
    GtkWidget *stack = gtk_stack_new();
    gtk_stack_set_transition_type(GTK_STACK(stack), GTK_STACK_TRANSITION_TYPE_SLIDE_UP_DOWN);//The effect of switching is upward
	gtk_stack_sidebar_set_stack(GTK_STACK_SIDEBAR(sidebar), GTK_STACK(stack));
    gtk_box_pack_start(GTK_BOX(hbox), gtk_separator_new(GTK_ORIENTATION_VERTICAL), FALSE, FALSE, 0);     


    stack = textAction(&stack);                          
	gtk_box_pack_start(GTK_BOX(hbox), stack, TRUE, TRUE, 0);
   

    //gtk_label_set_selectable(block, TRUE)

    gtk_container_add(GTK_CONTAINER(main_window), hbox);
    gtk_widget_show_all(main_window);
}
