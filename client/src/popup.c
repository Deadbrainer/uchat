#include "../inc/client.h"

void popup_add_chat()
{
    GtkWidget *roomname_hbox_error;
    // create components
    GtkWidget *add_chat_dialog = gtk_dialog_new_with_buttons("Create new chat",
                                                             GTK_WINDOW(main_window),
                                                             GTK_DIALOG_MODAL,
                                                             ("Create"),
                                                             GTK_RESPONSE_OK,
                                                             ("Cancel"),
                                                             GTK_RESPONSE_REJECT,
                                                             NULL);

    GtkWidget *add_chat_label = gtk_label_new("Enter chat name");
    GtkWidget *add_chat_entry = gtk_entry_new();
    roomname_error_label = gtk_label_new(NULL);
    GtkWidget *add_chat_content_area = gtk_dialog_get_content_area(GTK_DIALOG(add_chat_dialog));

    // packing
    gtk_label_set_width_chars(GTK_LABEL(roomname_error_label), 10);
    roomname_hbox_error = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_container_set_border_width(GTK_CONTAINER(add_chat_content_area), 10);

    gtk_box_pack_start(GTK_BOX(add_chat_content_area), add_chat_entry, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(add_chat_content_area), add_chat_label, TRUE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(roomname_hbox_error), roomname_error_label, TRUE, FALSE, 5);
    gtk_box_pack_end(GTK_BOX(add_chat_content_area), roomname_hbox_error, FALSE, FALSE, 5);
    apply_css(add_chat_dialog, provider);
    // show all
    gtk_widget_show_all(add_chat_content_area);

    // listener
    gint result = gtk_dialog_run(GTK_DIALOG(add_chat_dialog));
    char *to_send = "\r\v";
    int sock = 0;
    get_sockid(&sock, 0);
    switch (result)
    {
    case GTK_RESPONSE_OK:
        if (gtk_entry_get_text(GTK_ENTRY(add_chat_entry)) != NULL)
        {
            to_send = mx_strjoin(to_send, (char *)gtk_entry_get_text(GTK_ENTRY(add_chat_entry)));
            send(sock, to_send, mx_strlen(to_send), 0);
            char *rec = mx_strnew(8);
            recv(sock, rec, 8, 0);
            if (mx_strcmp(rec, "N") == 0)
            {
                gtk_label_set_markup(GTK_LABEL(roomname_error_label), "<span foreground='#ff0000'>Roomname existed</span>");
            }
            else
            {
                handle((char *)gtk_entry_get_text(GTK_ENTRY(add_chat_entry)));
            }
            mx_strdel(&rec);
        }
        else
        {
            gtk_label_set_markup(GTK_LABEL(roomname_error_label), "<span foreground='#ff0000'>Roomname haven`t been entered</span>");
        }
        break;
    case GTK_RESPONSE_REJECT:
        break;
    default:
        break;
    }

    // end
    gtk_widget_destroy(add_chat_dialog);
}

void popup_add_user(gpointer data)
{
    gpointer a = data;
    a = NULL;

    GtkWidget *add_user_hbox_error;
    // create components
    GtkWidget *add_user_dialog = gtk_dialog_new_with_buttons("Add new user",
                                                             GTK_WINDOW(main_window),
                                                             GTK_DIALOG_MODAL,
                                                             ("Ok"),
                                                             GTK_RESPONSE_OK,
                                                             ("Cancel"),
                                                             GTK_RESPONSE_REJECT,
                                                             NULL);

    GtkWidget *add_user_label = gtk_label_new("Enter nickname");
    GtkWidget *add_user_entry = gtk_entry_new();
    add_user_error_label = gtk_label_new(NULL);
    GtkWidget *add_user_content_area = gtk_dialog_get_content_area(GTK_DIALOG(add_user_dialog));

    gtk_label_set_width_chars(GTK_LABEL(add_user_error_label), 10);
    add_user_hbox_error = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

    // packing
    gtk_container_set_border_width(GTK_CONTAINER(add_user_content_area), 10);
    gtk_box_pack_start(GTK_BOX(add_user_content_area), add_user_entry, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(add_user_content_area), add_user_label, TRUE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(add_user_hbox_error), add_user_error_label, TRUE, FALSE, 5);
    gtk_box_pack_end(GTK_BOX(add_user_content_area), add_user_hbox_error, FALSE, FALSE, 5);

    apply_css(add_user_dialog, provider);
    // show all
    gtk_widget_show_all(add_user_content_area);

    // listener
    gint result = gtk_dialog_run(GTK_DIALOG(add_user_dialog));
    int sock = 0;
    get_sockid(&sock, 0);
    char *to_send = "\r\r\v";
    switch (result)
    {
    case GTK_RESPONSE_OK:
        if (gtk_entry_get_text(GTK_ENTRY(add_user_entry)) != NULL)
        {
            to_send = mx_strjoin(to_send, (char *)gtk_entry_get_text(GTK_ENTRY(add_user_entry)));
            const gchar *current_chat = gtk_stack_get_visible_child_name(GTK_STACK(chat_stack));
            to_send = mx_strjoin(to_send, "\v");
            to_send = mx_strjoin(to_send, current_chat);
            send(sock, to_send, mx_strlen(to_send), 0);
            char *rec = mx_strnew(8);
            recv(sock, rec, 8, 0);
            if (mx_strcmp(rec, "N") == 0)
            {
                gtk_label_set_markup(GTK_LABEL(add_user_error_label), "<span foreground='#ff0000'>Room doesn`t exist</span>");
            }
            else if (mx_strcmp(rec, "NN") == 0)
            {
                gtk_label_set_markup(GTK_LABEL(add_user_error_label), "<span foreground='#ff0000'>User is already invited</span>");
            }
            else if (mx_strcmp(rec, "NNN") == 0)
            {
                gtk_label_set_markup(GTK_LABEL(add_user_error_label), "<span foreground='#ff0000'>Username is wrong</span>");
            }
            mx_strdel(&rec);
        }
        else
        {
            gtk_label_set_markup(GTK_LABEL(add_user_error_label), "<span foreground='#ff0000'>Roomname haven`t been entered</span>");
        }
        break;
    case GTK_RESPONSE_REJECT:
        break;
    default:
        break;
    }

    // end
    gtk_widget_destroy(add_user_dialog);
}
