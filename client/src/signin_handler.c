#include "../inc/client.h"

void login_clicked_username(GtkWidget *button, gpointer data)
{
    int sock = 0;
    get_sockid(&sock, 0);
    char *username = NULL;
    username = (char *)gtk_entry_get_text(GTK_ENTRY((GtkWidget *)data));
    get_login(&username, 1);
}

void login_clicked_password(GtkWidget *button, gpointer data)
{
    int sock = 0;
    get_sockid(&sock, 0);
    char *password = NULL;
    password = (char *)gtk_entry_get_text(GTK_ENTRY((GtkWidget *)data));

    char *username;
    get_login(&username, 0);
    if (mx_strcmp(username, "") != 0)
    {
        username = mx_strjoin("L ", username);
        username = mx_strjoin(username, " ");
        username = mx_strjoin(username, password);
        if (mx_strcmp(password, "") != 0)
        {
            if (send(sock, username, mx_strlen(username), 0) < 0)
            {
                fprintf(stderr, "sending failure\n");
            }

            int len = 0;
            char *rec = mx_strnew(8);

            len = recv(sock, rec, 3, 0);

            if (mx_strcmp(rec, "YY") == 0)
            {
                main_menu_test();
            }
            else if ((mx_strcmp(rec, "YN") == 0))
            {
                gtk_label_set_markup(GTK_LABEL(log_error_label), "<span foreground='#ff0000'>Wrong password</span>");
            }
            else if ((mx_strcmp(rec, "NN") == 0))
            {
                gtk_label_set_markup(GTK_LABEL(log_error_label), "<span foreground='#ff0000'>Wrong username or password</span>");
            }
            else
            {
                printf("Unexpected answer: %s\n", rec);
            }
        }
    }
    else
    {
        gtk_label_set_markup(GTK_LABEL(log_error_label), "<span foreground='#ff0000'>Enter username and password</span>");
    }
}