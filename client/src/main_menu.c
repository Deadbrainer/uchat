#include "../inc/client.h"

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

void get_buffer(GtkTextBuffer **buffer, bool check)
{
    static GtkTextBuffer *t;
    if (check)
    {
        t = *buffer;
    }
    else
    {
        *buffer = t;
    }
}

void get_iter(GtkTextIter *iter, bool check)
{
    static GtkTextIter t;
    if (check)
    {
        t = *iter;
    }
    else
    {
        *iter = t;
    }
}

// void get_array(char **array, int check)
// { // to an array from main
//     static char *n;

//     if (check)
//     {
//         n = *array;
//     }
//     else
//     {
//         *array = n;
//     }
// }

void *recvmg_new(void *my_sock)
{
    printf("Waintig for messages to write\n");
    char *msg = mx_strnew(500);

    //char* msg = mx_strnew(500);
    //get_array(&msg, 0);

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
    //memset(msg, 500, '\0');

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

void main_menu()
{
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

    //gtk_label_set_selectable(block, TRUE)

    gtk_container_add(GTK_CONTAINER(main_window), vbox);
    gtk_widget_show_all(main_window);
}
