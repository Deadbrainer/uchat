#include "../inc/client.h"
GtkWidget *edit_window;

GMainContext *context;
//GDK_BUTTON_SECONDARY
int is_esc(GtkWidget *edit_window, GdkEventKey *event)
{
    if (event->keyval == GDK_KEY_Escape)
    {
        gtk_window_set_modal(GTK_WINDOW(edit_window), false);
        gtk_widget_destroy(edit_window);
    }
    return false;
}

gboolean local_edit_message()
{

    return G_SOURCE_REMOVE;
}
//
int ebr(GtkWidget *edit_entry, GdkEventKey *event, gpointer lineid)
{
    gint *b_line = (gint *)lineid;
    gint line = *b_line;
    printf("%d\n", line);
    if (event->keyval == GDK_KEY_KP_Enter || event->keyval == GDK_KEY_Return)
    {
        GtkWidget *vbox = gtk_stack_get_visible_child(GTK_STACK(chat_stack));
        GList *list1 = gtk_container_get_children(GTK_CONTAINER(vbox));
        GtkContainer *scroll = GTK_CONTAINER(list1->next->data);
        GList *list2 = gtk_container_get_children(GTK_CONTAINER(scroll));
        GtkWidget *textArea = GTK_WIDGET(list2->data);

        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textArea));
        GtkTextTagTable *table = gtk_text_buffer_get_tag_table(buffer);
        GtkTextTag *color = gtk_text_tag_table_lookup(table, "color");
        GtkTextTag *above_below_pixels = gtk_text_tag_table_lookup(table, "above_below_pixels");
        GtkTextTag *time_spacing = gtk_text_tag_table_lookup(table, "time_spacing");

        GtkTextIter line_iter;
        GtkTextMark *cursor;
        cursor = gtk_text_buffer_get_mark(buffer, "insert");
        gtk_text_buffer_get_iter_at_mark(buffer, &line_iter, cursor);
        gtk_text_iter_set_line(&line_iter, line);
        while (gtk_text_iter_starts_tag(&line_iter, above_below_pixels))
        {
            gtk_text_iter_forward_char(&line_iter);
        }
        gtk_text_iter_forward_char(&line_iter);
        GtkTextIter *liter = gtk_text_iter_copy(&line_iter);
        GtkTextIter *riter = gtk_text_iter_copy(&line_iter);
        while (!gtk_text_iter_ends_tag(riter, above_below_pixels))
        {
            gtk_text_iter_forward_char(riter);
        }
        gtk_text_buffer_delete(buffer, liter, riter);
        gtk_text_iter_set_line(&line_iter, line);
        while (!gtk_text_iter_ends_tag(&line_iter, color))
        {
            gtk_text_iter_forward_char(&line_iter);
        }
        const gchar *text = gtk_entry_get_text(GTK_ENTRY(edit_entry));
        gtk_text_buffer_insert_with_tags(buffer, &line_iter, text, -1, time_spacing, NULL);
        GtkTextIter *iditer = gtk_text_iter_copy(&line_iter);
        gtk_text_iter_forward_line(iditer);
        GtkTextIter *liditer = gtk_text_iter_copy(iditer);
        GtkTextIter *riditer = gtk_text_iter_copy(iditer);
        while (!gtk_text_iter_starts_tag(liditer, time_spacing))
        {
            gtk_text_iter_backward_char(liditer);
        }
        gchar *idtext = gtk_text_iter_get_text(liditer, riditer);
        char *to_send = mx_strjoin("\r\r\r\r\r\r\v", idtext);
        to_send = mx_strjoin(to_send, "\v");
        to_send = mx_strjoin(to_send, text);
        int sock = 0;
        get_sockid(&sock, 0);
        send(sock, to_send, mx_strlen(to_send), 0);
        gtk_window_set_modal(GTK_WINDOW(edit_window), false);
        gtk_widget_destroy(edit_window);
    }
    return false;
}

void edit_button_reader(const gchar *text, gint lineid)
{
    edit_window = gtk_window_new(GTK_WINDOW_POPUP);
    new_window(edit_window, 400, -1, false, 0, "uchat");
    gtk_window_set_modal(GTK_WINDOW(edit_window), true);
    //GtkWidget *edit_label = gtk_label_new(text);
    GtkWidget *edit_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(edit_entry), text);
    gint *line_id = &lineid;
    g_signal_connect(G_OBJECT(edit_entry), "key-press-event", G_CALLBACK(ebr), (gpointer)line_id);
    g_signal_connect(G_OBJECT(edit_window), "key-press-event", G_CALLBACK(is_esc), NULL);

    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);
    gtk_box_pack_start(GTK_BOX(hbox), edit_entry, false, false, 5);

    gtk_container_add(GTK_CONTAINER(edit_window), hbox);
    gtk_widget_show_all(edit_window);
}

void edit_message(gint lineid)
{
    GtkWidget *vbox = gtk_stack_get_visible_child(GTK_STACK(chat_stack));
    GList *list1 = gtk_container_get_children(GTK_CONTAINER(vbox));
    GtkContainer *scroll = GTK_CONTAINER(list1->next->data);
    GList *list2 = gtk_container_get_children(GTK_CONTAINER(scroll));
    GtkWidget *textArea = GTK_WIDGET(list2->data);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textArea));
    GtkTextTagTable *table = gtk_text_buffer_get_tag_table(buffer);
    GtkTextTag *color = gtk_text_tag_table_lookup(table, "color");
    GtkTextTag *above_below_pixels = gtk_text_tag_table_lookup(table, "above_below_pixels");
    GtkTextIter line_iter;
    GtkTextMark *cursor;
    cursor = gtk_text_buffer_get_mark(buffer, "insert");
    gtk_text_buffer_get_iter_at_mark(buffer, &line_iter, cursor);
    gtk_text_iter_set_line(&line_iter, lineid);
    //gtk_text_iter_forward_char(line_iter); //? placeholder
    while (!gtk_text_iter_ends_tag(&line_iter, color))
    {
        gtk_text_iter_forward_char(&line_iter);
    }
    gtk_text_iter_forward_char(&line_iter);

    GtkTextIter *liter;
    liter = gtk_text_iter_copy(&line_iter);
    GtkTextIter *riter;
    riter = gtk_text_iter_copy(&line_iter);

    while (!gtk_text_iter_ends_tag(riter, above_below_pixels))
    {
        gtk_text_iter_forward_char(riter);
    }
    gtk_text_iter_backward_char(liter);

    const gchar *text = gtk_text_iter_get_text(liter, riter);
    edit_button_reader(text, lineid);
}

gboolean show_message(gpointer message)
{
    char **splited_msg = mx_strsplit(message, '\t'); // 0 - name, 1 - text, 2 - date, 3 - id
    if (mx_strcmp(splited_msg[0], "") != 0 && splited_msg[0] != NULL)
    {
        GtkWidget *vbox = gtk_stack_get_visible_child(GTK_STACK(chat_stack));
        GList *list1 = gtk_container_get_children(GTK_CONTAINER(vbox));
        GtkContainer *scroll = GTK_CONTAINER(list1->next->data);
        GList *list2 = gtk_container_get_children(GTK_CONTAINER(scroll));
        GtkWidget *textArea = GTK_WIDGET(list2->data);

        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textArea));
        GtkTextTagTable *table = gtk_text_buffer_get_tag_table(buffer);
        GtkTextTag *invtag = gtk_text_tag_table_lookup(table, "inv");
        GtkTextTag *above_pixels = gtk_text_tag_table_lookup(table, "above_pixels");
        GtkTextTag *color = gtk_text_tag_table_lookup(table, "color");
        GtkTextTag *time_spacing = gtk_text_tag_table_lookup(table, "time_spacing");

        GtkTextIter iter;
        GtkTextMark *cursor;
        cursor = gtk_text_buffer_get_mark(buffer, "insert");
        gtk_text_buffer_get_iter_at_mark(buffer, &iter, cursor);
        gtk_text_iter_forward_to_end(&iter);
        gtk_text_buffer_place_cursor(buffer, &iter);

        const gchar *name = splited_msg[0];
        const gchar *text = splited_msg[1];
        const gchar *date = splited_msg[2];
        const gchar *textid = splited_msg[3];

        gtk_text_buffer_insert_with_tags(buffer, &iter, name, -1, color, above_pixels, NULL);
        gtk_text_buffer_insert_with_tags(buffer, &iter, text, -1, above_pixels, NULL);
        gtk_text_buffer_insert_with_tags(buffer, &iter, date, -1, time_spacing, above_pixels, NULL);
        gtk_text_buffer_insert_with_tags(buffer, &iter, textid, -1, invtag, above_pixels, NULL);

        gtk_text_buffer_insert(buffer, &iter, "\n", -1);
        get_buffer(&buffer, 1);
    }
    auto_scroll();
    return G_SOURCE_REMOVE;
}

int on_button_press(GtkWidget *widget, GdkEventButton *event)
{
    widget = NULL;
    GtkWidget *vbox = gtk_stack_get_visible_child(GTK_STACK(chat_stack));
    GList *list1 = gtk_container_get_children(GTK_CONTAINER(vbox));
    GtkContainer *scroll = GTK_CONTAINER(list1->next->data);
    GList *list2 = gtk_container_get_children(GTK_CONTAINER(scroll));
    GtkWidget *textArea = GTK_WIDGET(list2->data);
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textArea));

    if (event->type == GDK_2BUTTON_PRESS)
    {
        GtkTextIter iter;
        GtkTextMark *cursor = gtk_text_buffer_get_mark(buffer, "insert");
        gtk_text_buffer_get_iter_at_mark(buffer, &iter, cursor);
        gint line_id = gtk_text_iter_get_line(&iter);
        edit_message(line_id);

        //gchar *text = gtk_text_buffer_get_text(buffer, &liter, &riter, false); //!
        // gtk_text_buffer_insert(buffer, riter, "Test for replacement", strlen("Test for replacement"));

        // gtk_text_buffer_get_iter_at_mark(buffer, liter, cursor);
        // gtk_text_buffer_get_iter_at_mark(buffer, riter, cursor);
        // gtk_text_iter_backward_line(liter);
        // gtk_text_iter_forward_line(liter);
        // gtk_text_iter_forward_line(riter);
        // gtk_text_iter_backward_char(riter);
        // gtk_text_iter_backward_chars(riter, strlen("Test for replacement"));

        // gtk_text_buffer_delete(buffer, liter, &riter);
    }
    else if (current_roomname != gtk_stack_get_visible_child_name(GTK_STACK(chat_stack)))
    {
        gtk_text_buffer_set_text(buffer, "", -1);

        current_roomname = gtk_stack_get_visible_child_name(GTK_STACK(chat_stack));
        int sock = 0;
        get_sockid(&sock, 0);
        send(sock, mx_strjoin("\r\r\r\r\r\v", current_roomname), mx_strlen(mx_strjoin("\r\r\r\r\r\v", current_roomname)), 0);
    }
    return false;
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
    GtkWidget *a = text_entry;
    a = NULL;
    
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

    GtkWidget *chat_text_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    style_context = gtk_widget_get_style_context(chat_text_hbox);
    gtk_style_context_add_class(style_context, "chat_text_hbox");

    GtkWidget *textArea = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(textArea), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(textArea), FALSE);
    gtk_text_view_set_top_margin(GTK_TEXT_VIEW(textArea), 10);
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(textArea), 10);
    gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(textArea), 10);
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(textArea), 10);

    gtk_widget_add_events(textArea, GDK_BUTTON_PRESS_MASK);
    g_signal_connect(G_OBJECT(textArea), "button-press-event", G_CALLBACK(on_button_press), NULL);

    GtkTextBuffer *buffer;
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textArea));

    GtkTextTag *invtag;
    invtag = gtk_text_buffer_create_tag(buffer, "inv", "invisible", true, NULL);
    GtkTextTag *color;
    color = gtk_text_buffer_create_tag(buffer, "color", "invisible", false, NULL); //! Change to rgba
    GtkTextTag *above_pixels;
    above_pixels = gtk_text_buffer_create_tag(buffer, "above_pixels", "pixels_above_lines", 15, /*"pixels_below_lines", 5,*/ NULL);
    GtkTextTag *time_spacing;
    time_spacing = gtk_text_buffer_create_tag(buffer, "time_spacing", "invisible", false, NULL); //!Change to correct spacing

    GtkWidget *scrolledwindow = gtk_scrolled_window_new(NULL, NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *chat_text_add_button = gtk_button_new_with_label(" ");
    g_signal_connect(G_OBJECT(chat_text_add_button), "clicked", G_CALLBACK(popup_add_user), NULL);
    style_context = gtk_widget_get_style_context(chat_text_add_button);
    gtk_style_context_add_class(style_context, "chat_text_add_button");

    GtkWidget *textEntry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(textEntry), "Write a message");

    GtkWidget *chat_text_send_button = gtk_button_new_with_label("Send");

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
        }
        else if (msg[4] == '\r' && msg[5] != '\r')
        {
            char **splited = mx_strsplit(msg, '\v');
            gchar *gtext = splited[1];
            source = g_idle_source_new();
            g_source_set_callback(source, local_edit_message, gtext, NULL); //! Naxyia id blyat togda
            g_source_attach(source, context);
            g_source_unref(source);
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

    g_signal_connect(G_OBJECT(chat_hbox), "button-release-event", G_CALLBACK(on_button_press), NULL);
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
