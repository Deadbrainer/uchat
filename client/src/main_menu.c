#include "../inc/client.h"

GMainContext *context;
//GDK_BUTTON_SECONDARY
gboolean show_message(gpointer message)
{
    GtkWidget *vbox = gtk_stack_get_visible_child(GTK_STACK(chat_stack));
    GList *list1 = gtk_container_get_children(GTK_CONTAINER(vbox));
    GtkContainer *scroll = GTK_CONTAINER(list1->data);
    GList *list2 = gtk_container_get_children(GTK_CONTAINER(scroll));
    GtkWidget *textArea = GTK_WIDGET(list2->data);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textArea));
    GtkTextIter iter;
    GtkTextMark *cursor;

    cursor = gtk_text_buffer_get_mark(buffer, "insert");
    gtk_text_buffer_get_iter_at_mark(buffer, &iter, cursor);
    gtk_text_iter_forward_to_end(&iter);
    gtk_text_buffer_place_cursor(buffer, &iter);

    gtk_text_buffer_insert(buffer, &iter, message, -1);
    gtk_text_buffer_insert(buffer, &iter, "\n\n", -1);
    return G_SOURCE_REMOVE;
}

int on_button_press(GtkWidget *widget, GdkEventButton *event)
{
    if (event->type == GDK_BUTTON_PRESS)
    {
        if (gtk_stack_get_transition_running(GTK_STACK(chat_stack)))
        {
            current_roomname = gtk_stack_get_visible_child_name(GTK_STACK(chat_stack));
        }
    }
    return false;
}

int on_key_press(GtkWidget *text_entry, GdkEventKey *event, GtkTextView *textview)
{
    if (event->keyval == GDK_KEY_KP_Enter || event->keyval == GDK_KEY_Return)
    {
        GtkTextIter iter;
        GtkTextMark *cursor;
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));

        const gchar *text = gtk_entry_get_text(GTK_ENTRY(text_entry));

        int sock = 0;
        get_sockid(&sock, 0);

        t_list *roomnames = NULL;
        get_list(&roomnames, 0);

        const gchar *current_chat = gtk_stack_get_visible_child_name(GTK_STACK(chat_stack));
        char *to_send = mx_strjoin("\r\r\r\v", current_chat);
        to_send = mx_strjoin(to_send, "\v");
        if (send(sock, mx_strjoin(to_send, text), strlen(mx_strjoin(to_send, text)), 0) < 0)
        {
            fprintf(stderr, "sending failure\n");
        }

        cursor = gtk_text_buffer_get_mark(buffer, "insert");
        gtk_text_buffer_get_iter_at_mark(buffer, &iter, cursor);
        gtk_text_iter_forward_to_end(&iter);
        get_iter(&iter, 1); //!assign
        gtk_text_buffer_place_cursor(buffer, &iter);
        char *date = get_date();
        char *username = NULL;
        get_login(&username, 0);
        while (1)
        {
            if (username != NULL)
            {
                break;
            }
            get_login(&username, 0);
            sleep(1);
        }
        const gchar *show_text = NULL;
        show_text = mx_strjoin(username, ": ");
        show_text = mx_strjoin(show_text, text);
        show_text = mx_strjoin(show_text, " ");
        show_text = mx_strjoin(show_text, date);
        gtk_text_buffer_insert(buffer, &iter, show_text, -1);

        gtk_text_buffer_insert(buffer, &iter, "\n\n", -1);
        gtk_entry_set_text(GTK_ENTRY(text_entry), "");
    }
    return false;
}

GtkWidget *add_chat(GtkWidget **stack, char *text)
{
    GtkWidget *textArea = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(textArea), false);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(textArea), false);

    GtkTextBuffer *buffer;
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textArea));
    GtkTextTag *tag = gtk_text_buffer_create_tag(buffer, "edit", "editable", true, NULL);

    GtkWidget *scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrolledwindow), textArea);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), scrolledwindow, true, true, 5);

    GtkWidget *button = gtk_button_new_with_label("Invite Friend");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(popup_add_user), NULL);

    GtkWidget *textEntry = gtk_entry_new();
    g_signal_connect(G_OBJECT(textEntry), "key-press-event", G_CALLBACK(on_key_press), textArea);

    GtkWidget *hbox2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(hbox2), textEntry, true, true, 5);
    gtk_box_pack_start(GTK_BOX(hbox2), button, false, false, 0);
    gtk_box_pack_end(GTK_BOX(vbox), hbox2, FALSE, FALSE, 5);
    gtk_stack_add_named(GTK_STACK(*stack), vbox, text);
    gtk_container_child_set(GTK_CONTAINER(*stack), vbox, "title", text, NULL);
    return *stack;
}

gboolean handle(gpointer user_data)
{
    chat_stack = add_chat(&chat_stack, user_data);
    gtk_widget_show_all(main_window);
    return G_SOURCE_REMOVE;
}

void *recvmg_new(void *my_sock)
{
    GSource *source;

    char *msg = mx_strnew(500);

    int sock = *((int *)my_sock);
    int len;

    while ((len = recv(sock, msg, 500, 0)) > 0)
    {
        if (msg[0] == '\r' && msg[1] != '\r')
        {
            char **splited = mx_strsplit(msg, '\v');
            char **splited_final = mx_strsplit(splited[1], '\t');
            gchar *gtext = splited_final[0];
            source = g_idle_source_new();
            g_source_set_callback(source, handle, gtext, NULL);
            g_source_attach(source, context);
            g_source_unref(source);
        }
        else if (msg[1] == '\r' && msg[2] != '\r')
        {
            char **splited = mx_strsplit(msg, '\v');
            gchar *gtext = splited[1];
            source = g_idle_source_new();
            g_source_set_callback(source, handle, gtext, NULL);
            g_source_attach(source, context);
            g_source_unref(source);
        }
        else if (msg[2] == '\r' && msg[3] != '\r')
        {
            char **splited = mx_strsplit(msg, '\v');
            current_roomname = gtk_stack_get_visible_child_name(GTK_STACK(chat_stack));
            if (mx_strcmp(splited[1], current_roomname) == 0)
            {
                gchar *gtext = splited[2];
                source = g_idle_source_new();
                g_source_set_callback(source, show_message, gtext, NULL);
                g_source_attach(source, context);
                g_source_unref(source);
            }
        }
        else if (msg[3] == '\r')
        {
            char **splited_name = mx_strsplit(msg, '\v');
            printf("%s\n", splited_name[1]);
            get_login(&splited_name[1], 1);
        }

        bzero(msg, 500);
    }

    //free(&msg);

    return NULL; // to silence warning
}

void main_menu()
{
    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    new_window(main_window, 1000, 800, false, 0, "uchat");
    gtk_widget_add_events(main_window, GDK_BUTTON_PRESS_MASK);
    g_signal_connect(G_OBJECT(main_window), "button-press-event", G_CALLBACK(on_button_press), NULL);

    // sidebar init
    GtkWidget *chat_sidebar = gtk_stack_sidebar_new();
    chat_stack = gtk_stack_new();
    gtk_stack_sidebar_set_stack(GTK_STACK_SIDEBAR(chat_sidebar), GTK_STACK(chat_stack));

    GtkWidget *chat_separator = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
    GtkWidget *chat_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *chat_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *chat_add_button = gtk_button_new_with_label("Add chat");
    g_signal_connect(G_OBJECT(chat_add_button), "clicked", G_CALLBACK(popup_add_chat), NULL);

    // packing
    gtk_box_pack_start(GTK_BOX(chat_vbox), chat_sidebar, TRUE, TRUE, 0);
    gtk_box_pack_end(GTK_BOX(chat_vbox), chat_add_button, FALSE, FALSE, 5);

    gtk_box_pack_start(GTK_BOX(chat_hbox), chat_vbox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(chat_hbox), chat_separator, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(chat_hbox), chat_stack, TRUE, TRUE, 0);

    context = g_main_context_default();

    int sock = 0;
    get_sockid(&sock, 0);

    pid = getpid();
    GThread *thread;
    thread = g_thread_new(NULL, recvmg_new, &sock);

    gtk_container_add(GTK_CONTAINER(main_window), chat_hbox);
    gtk_widget_show_all(main_window);
}
