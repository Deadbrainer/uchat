#include "../inc/client.h"

void *recvmg_new(void *my_sock)
{
    printf("Waintig for messages to write\n");
    char *msg = mx_strnew(500);

    int sock = *((int *)my_sock);
    int len;
    while ((len = recv(sock, msg, 500, 0)) > 0)
    {
        printf("ZDES: %s\n", msg);
        //msg[len] = '\0';
        // fputs(msg, stdout);
        GtkTextBuffer *buffer;
        GtkTextIter iter;
        GtkTextMark *cursor;

        get_buffer(&buffer, 0);
        //get_iter(&iter, 0);
        cursor = gtk_text_buffer_get_mark(buffer, "insert");
        gtk_text_buffer_get_iter_at_mark(buffer, &iter, cursor);
        gtk_text_iter_forward_to_end(&iter);
        gtk_text_buffer_place_cursor(buffer, &iter);
        gtk_text_buffer_insert(buffer, &iter, msg, -1);
        gtk_text_buffer_insert(buffer, &iter, "\n\n", -1);

        printf("Clearing\n");
        bzero(msg, 500);
        printf("Buf is: %s\n", msg);
    }

    free(&msg);

    return NULL; // to silence warning
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

        int sock = 0;
        get_sockid(&sock, 0);
        fprintf(stderr, "%d\n", sock);

        //printf("%s\n", text);

        if (send(sock, text, strlen(text), 0) < 0)
        {
            fprintf(stderr, "sending failure\n");
        }

        cursor = gtk_text_buffer_get_mark(buffer, "insert");
        gtk_text_buffer_get_iter_at_mark(buffer, &iter, cursor);
        gtk_text_iter_forward_to_end(&iter);
        get_iter(&iter, 1); //!assign
        gtk_text_buffer_place_cursor(buffer, &iter);

        gtk_text_buffer_insert(buffer, &iter, text, -1);

        gtk_text_buffer_insert(buffer, &iter, "\n\n", -1);
        gtk_entry_set_text(GTK_ENTRY(textEntry), "");
    }
    return false;
}

void add_chat(const gchar *title, gpointer data)
{
    GtkWidget *stack = (GtkWidget *)data;

    GtkWidget *label = gtk_label_new(title);
    gtk_stack_add_named(GTK_STACK(stack), label, title);
    gtk_container_child_set(GTK_CONTAINER(stack), label, "title", title, NULL);
}

void main_menu()
{
    GtkWidget *sidebar = gtk_stack_sidebar_new();
    GtkWidget *stack = gtk_stack_new();
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_VERTICAL);

    GtkWidget *textArea = gtk_text_view_new();
    GtkWidget *scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
    GtkWidget *textEntry = gtk_entry_new();
    get_text_entry(&textEntry, 1); //!assign
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    GtkTextBuffer *buffer;
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textArea));
    get_buffer(&buffer, 1); //!assign

    //textview parametres
    gtk_text_view_set_editable(GTK_TEXT_VIEW(textArea), false);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(textArea), false);

    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    new_window(main_window, 1000, 800, TRUE, 10, "uchat");
    gtk_widget_add_events(main_window, GDK_KEY_PRESS_MASK); //* key scanning

    sidebar = gtk_stack_sidebar_new();
    stack = gtk_stack_new();
    separator = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    gtk_stack_set_transition_type(GTK_STACK(stack), GTK_STACK_TRANSITION_TYPE_SLIDE_UP_DOWN);
    gtk_stack_sidebar_set_stack(GTK_STACK_SIDEBAR(sidebar), GTK_STACK(stack));

    // packing
    gtk_box_pack_start(GTK_BOX(hbox), sidebar, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), separator, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), stack, TRUE, TRUE, 0);

    int sock = 0;
    get_sockid(&sock, 0);
    fprintf(stderr, "%d\n", sock);

    pthread_t recvt;
    pthread_create(&recvt, NULL, (void *)recvmg_new, &sock);

    g_signal_connect(G_OBJECT(main_window), "delete-event", G_CALLBACK(closeApp), NULL);
    //g_signal_connect_swapped(G_OBJECT(textEntry), "key-press-event", G_CALLBACK(on_key_press), textEntry);
    g_signal_connect(G_OBJECT(textEntry), "key-press-event", G_CALLBACK(on_key_press), buffer);

    gtk_container_add(GTK_CONTAINER(scrolledwindow), textArea);
    gtk_box_pack_start(GTK_BOX(vbox), scrolledwindow, true, true, 5);
    gtk_box_pack_end(GTK_BOX(vbox), textEntry, FALSE, FALSE, 5);

    add_chat("dasdasda", stack);
    add_chat("das5235235dasda", stack);
    add_chat("dasdgdfgdfgdfgdfasda", stack);
    //gtk_label_set_selectable(block, TRUE)

    //gtk_container_add(GTK_CONTAINER(main_window), vbox);
    gtk_container_add(GTK_CONTAINER(main_window), hbox);
    gtk_widget_show_all(main_window);
}
