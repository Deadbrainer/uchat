#include "../inc/client.h"

void auto_scroll()
{
    GtkAdjustment *vadjustment;
    GtkWidget *vbox = gtk_stack_get_visible_child(GTK_STACK(chat_stack));
    GList *list1 = gtk_container_get_children(GTK_CONTAINER(vbox));
    GtkContainer *scroll = GTK_CONTAINER(list1->next->data);
    vadjustment = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(scroll));

    gtk_adjustment_set_value(vadjustment, (gtk_adjustment_get_upper(vadjustment) - gtk_adjustment_get_page_size(vadjustment)));
}

GMainContext *context;
//GDK_BUTTON_SECONDARY
gboolean show_message(gpointer message)
{
    printf("%s try to print\n", message);
    if (mx_strcmp(message, "") != 0 && message != NULL)
    {
        GtkWidget *vbox = gtk_stack_get_visible_child(GTK_STACK(chat_stack));
        GList *list1 = gtk_container_get_children(GTK_CONTAINER(vbox));
        GtkContainer *scroll = GTK_CONTAINER(list1->next->data);
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
        get_buffer(&buffer, 1);
    }
    return G_SOURCE_REMOVE;
}

int on_button_press(GtkWidget *widget, GdkEventButton *event)
{
    // TODO: Work in progress - edit
    // if(event->type == GDK_2BUTTON_PRESS)
    // {
    //     GtkTextIter liter;
    //     GtkTextIter riter;
    //     GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
    //     GtkTextMark *cursor = gtk_text_buffer_get_mark(buffer, "insert");
    //     gtk_text_buffer_get_iter_at_mark(buffer, &liter, cursor);
    //     gtk_text_buffer_get_iter_at_mark(buffer, &riter, cursor);
    //     gtk_text_iter_backward_line(&liter);
    //     gtk_text_iter_forward_line(&liter);
    //     gtk_text_iter_forward_line(&riter);
    //     gtk_text_iter_backward_char(&riter);

    //     gchar *text = gtk_text_buffer_get_text(buffer, &liter, &riter, false);
    //     gtk_text_buffer_insert(buffer, &riter, "Test for replacement", strlen("Test for replacement"));

    //     gtk_text_buffer_get_iter_at_mark(buffer, &liter, cursor);
    //     gtk_text_buffer_get_iter_at_mark(buffer, &riter, cursor);
    //     gtk_text_iter_backward_line(&liter);
    //     gtk_text_iter_forward_line(&liter);
    //     gtk_text_iter_forward_line(&riter);
    //     gtk_text_iter_backward_char(&riter);
    //     gtk_text_iter_backward_chars(&riter, strlen("Test for replacement"));

    //     gtk_text_buffer_delete(buffer, &liter, &riter);
    // }
    /*else*/ if (current_roomname != gtk_stack_get_visible_child_name(GTK_STACK(chat_stack)))
    {
        GtkWidget *vbox = gtk_stack_get_visible_child(GTK_STACK(chat_stack));
        GList *list1 = gtk_container_get_children(GTK_CONTAINER(vbox));
        GtkContainer *scroll = GTK_CONTAINER(list1->next->data);
        GList *list2 = gtk_container_get_children(GTK_CONTAINER(scroll));
        GtkWidget *textArea = GTK_WIDGET(list2->data);
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textArea));
        gtk_text_buffer_set_text(buffer, "", -1);

        current_roomname = gtk_stack_get_visible_child_name(GTK_STACK(chat_stack));
        int sock = 0;
        get_sockid(&sock, 0);
        send(sock, mx_strjoin("\r\r\r\r\r\v", current_roomname), mx_strlen(mx_strjoin("\r\r\r\r\r\v", current_roomname)), 0);
    }
    return false;
}

void try_reconnect()
{
    struct sockaddr_in *ServerIp = NULL;
    get_struct_socaddr(&ServerIp, 0);

    int sock;

    while (1)
    {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (connect(sock, (struct sockaddr *)ServerIp, sizeof(*ServerIp)) == -1)
        {
            fprintf(stderr, "Connection failure\n");
            sleep(1);
        }
        else
        {
            printf("Connected\n");
            get_sockid(&sock, 1);

            char *name;
            get_login(&name, 0);
            printf("%s\n", name);
            send(sock, mx_strjoin("\r\r\r\r\r\r\v", name), strlen(mx_strjoin("\r\r\r\r\r\r\v", name)), 0);
            recvmg_new(&sock);
            break;
        }
    }
}

void send_message(GtkWidget *text_entry, GtkTextView *textview)
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
    const gchar *show_text = NULL;
    show_text = mx_strjoin(username, ": ");
    show_text = mx_strjoin(show_text, text);
    show_text = mx_strjoin(show_text, " ");
    show_text = mx_strjoin(show_text, date);

    gtk_text_buffer_insert(buffer, &iter, show_text, -1);
    gtk_text_buffer_insert(buffer, &iter, "\n\n", -1);
    gtk_entry_set_text(GTK_ENTRY(text_entry), "");

    auto_scroll();
    usleep(2000);
}

int on_key_press(GtkWidget *text_entry, GdkEventKey *event, GtkTextView *textview)
{
    if (event->keyval == GDK_KEY_KP_Enter || event->keyval == GDK_KEY_Return)
    {
        send_message(text_entry, textview);
    }
    return false;
}

void send_message_btn(GtkWidget *text_entry, GtkTextView *textview)
{
    GtkWidget *vbox = gtk_stack_get_visible_child(GTK_STACK(chat_stack));
    GList *list1 = gtk_container_get_children(GTK_CONTAINER(vbox));
    GtkContainer *hbox = GTK_CONTAINER(list1->next->next->data);
    GList *list2 = gtk_container_get_children(GTK_CONTAINER(hbox));
    GtkWidget *textEntry = GTK_WIDGET(list2->data);
    send_message(textEntry, textview);
}

GtkWidget *add_chat(GtkWidget **stack, char *text)
{
    GtkWidget *chat_text_label_top;
    GtkWidget *chat_text_top_hbox;

    chat_text_top_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    chat_text_label_top = gtk_label_new(text);
    style_context = gtk_widget_get_style_context(chat_text_label_top);
    gtk_style_context_add_class(style_context, "chat_text_label_top");

    chat_text_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    style_context = gtk_widget_get_style_context(chat_text_hbox);
    gtk_style_context_add_class(style_context, "chat_text_hbox");

    GtkWidget *textArea = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(textArea), TRUE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(textArea), TRUE);
    gtk_text_view_set_top_margin(GTK_TEXT_VIEW(textArea), 10);
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(textArea), 10);
    gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(textArea), 10);
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(textArea), 10);

    GtkTextBuffer *buffer;
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textArea));
    //GtkTextTag *tag = gtk_text_buffer_create_tag(buffer, "edit", "editable", true, NULL);

    GtkWidget *scrolledwindow = gtk_scrolled_window_new(NULL, NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *chat_text_add_button = gtk_button_new_with_label(" ");
    g_signal_connect(G_OBJECT(chat_text_add_button), "clicked", G_CALLBACK(popup_add_user), NULL);
    style_context = gtk_widget_get_style_context(chat_text_add_button);
    gtk_style_context_add_class(style_context, "chat_text_add_button");

    GtkWidget *textEntry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(textEntry), "Write a message");

    chat_text_send_button = gtk_button_new_with_label("Send");

    g_signal_connect(G_OBJECT(chat_text_send_button), "clicked", G_CALLBACK(send_message_btn), textArea);
    g_signal_connect(G_OBJECT(textEntry), "key-press-event", G_CALLBACK(on_key_press), textArea);

    gtk_container_add(GTK_CONTAINER(scrolledwindow), textArea);

    gtk_box_pack_start(GTK_BOX(chat_text_top_hbox), chat_text_label_top, FALSE, FALSE, 10);
    gtk_box_pack_end(GTK_BOX(chat_text_top_hbox), chat_text_add_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), chat_text_top_hbox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), scrolledwindow, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(chat_text_hbox), textEntry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(chat_text_hbox), chat_text_send_button, FALSE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(vbox), chat_text_hbox, FALSE, FALSE, 0);

    gtk_stack_add_named(GTK_STACK(*stack), vbox, text);
    gtk_container_child_set(GTK_CONTAINER(*stack), vbox, "title", text, NULL);

    apply_css(main_window, provider);
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
        else if (msg[3] == '\r' && msg[4] != '\r')
        {
            char **splited = mx_strsplit(msg, '\v');
            current_roomname = gtk_stack_get_visible_child_name(GTK_STACK(chat_stack));
            if (mx_strcmp(splited[1], current_roomname) == 0)
            {
                if (splited[2] != NULL && !(mx_isspace(*splited[2])) && *splited[2] != '\0')
                {
                    gchar *gtext = splited[2];
                    source = g_idle_source_new();
                    g_source_set_callback(source, show_message, gtext, NULL);
                    g_source_attach(source, context);
                    g_source_unref(source);
                }
            }
            //usleep(500);
        }

        bzero(msg, 500);
    }
    close(sock);
    try_reconnect();
    return NULL; // to silence warning
}

void main_menu()
{
    GtkWidget *chat_label_top;
    GtkWidget *chat_top_hbox;

    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    new_window(main_window, 1000, 800, false, 0, "uchat");
    gtk_widget_add_events(main_window, GDK_BUTTON_PRESS_MASK);

    // sidebar init
    chat_label_top = gtk_label_new(" YOUR CHATS ");
    style_context = gtk_widget_get_style_context(chat_label_top);
    gtk_style_context_add_class(style_context, "chat_label_top");

    GtkWidget *chat_sidebar = gtk_stack_sidebar_new();
    chat_stack = gtk_stack_new();

    chat_top_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    //gtk_stack_set_transition_type(GTK_STACK(chat_stack), GTK_STACK_TRANSITION_TYPE_SLIDE_UP_DOWN);
    gtk_stack_sidebar_set_stack(GTK_STACK_SIDEBAR(chat_sidebar), GTK_STACK(chat_stack));
    g_signal_connect(G_OBJECT(main_window), "button-release-event", G_CALLBACK(on_button_press), NULL);

    GtkWidget *chat_separator = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
    GtkWidget *chat_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *chat_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *chat_add_button = gtk_button_new_with_label("Add chat");
    g_signal_connect(G_OBJECT(chat_add_button), "clicked", G_CALLBACK(popup_add_chat), NULL);

    // packing
    gtk_box_pack_start(GTK_BOX(chat_vbox), chat_label_top, FALSE, FALSE, 12);
    gtk_box_pack_start(GTK_BOX(chat_vbox), chat_sidebar, TRUE, TRUE, 0);
    gtk_box_pack_end(GTK_BOX(chat_vbox), chat_add_button, FALSE, FALSE, 5);

    gtk_box_pack_start(GTK_BOX(chat_hbox), chat_vbox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(chat_hbox), chat_separator, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(chat_hbox), chat_stack, TRUE, TRUE, 0);

    g_signal_connect_after(G_OBJECT(main_window), "delete-event", G_CALLBACK(closeApp), NULL);

    //context = g_main_context_default();

    int sock = 0;
    get_sockid(&sock, 0);

    pid = getpid();
    GThread *thread;
    thread = g_thread_new(NULL, recvmg_new, &sock);

    gtk_container_add(GTK_CONTAINER(main_window), chat_hbox);
    style_context = gtk_widget_get_style_context(main_window);
    provider = GTK_STYLE_PROVIDER(gtk_css_provider_new());

    gtk_css_provider_load_from_path(GTK_CSS_PROVIDER(provider), "client/src/style.css", NULL);
    apply_css(main_window, provider);
    gtk_widget_show_all(main_window);
}