#include "../inc/client.h"

void reg_clicked_username(GtkWidget *button, gpointer data)
{
    int sock = 0;
    get_sockid(&sock, 0);
    char *username = (char *)gtk_entry_get_text(GTK_ENTRY((GtkWidget *)data));

    get_login(&username, 1);
}

void reg_clicked_password(GtkWidget *button, gpointer data)
{
    int sock = 0;
    get_sockid(&sock, 0);
    char *password = (char *)gtk_entry_get_text(GTK_ENTRY((GtkWidget *)data));

    get_password(&password, 1);
}

void reg_clicked_password_repeat(GtkWidget *button, gpointer data)
{
    int sock = 0;
    get_sockid(&sock, 0);
    char *password_repeat = (char *)gtk_entry_get_text(GTK_ENTRY((GtkWidget *)data));

    char *username;
    char *password;
    get_login(&username, 0);
    get_password(&password, 0);

    if (mx_strcmp(password, password_repeat) == 0)
    {
        username = mx_strjoin("R ", username);
        username = mx_strjoin(username, " ");
        username = mx_strjoin(username, password);

        if (send(sock, username, mx_strlen(username), 0) < 0)
        {
            fprintf(stderr, "sending failure\n");
        }

        int len = 0;
        char *rec = mx_strnew(8);

        len = recv(sock, rec, 3, 0);

        if (mx_strcmp(rec, "NN") == 0)
        {
            main_menu_test();
        }
        else if ((mx_strcmp(rec, "YN") == 0) || (mx_strcmp(rec, "YY") == 0))
        {
            get_error_label(&reg_error_label, 0);
            gtk_label_set_markup(GTK_LABEL(reg_error_label), "<span foreground='#ff0000'>Username has been already taken</span>");
        }
        else
        {
            printf("Unexpected answer: %s\n", rec);
        }
    }
    else
    {
        get_error_label(&reg_error_label, 0);
        gtk_label_set_markup(GTK_LABEL(reg_error_label), "<span foreground='#ff0000'>Passwords don`t match</span>");
    }
}
