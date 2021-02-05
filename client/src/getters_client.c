#include "../inc/client.h"

void get_error_label(GtkWidget **log_error_label, bool check)
{
    static GtkWidget *t;
    if (check)
    {
        t = *log_error_label;
    }
    else
    {
        *log_error_label = t;
    }
}

void get_if_login_ok(bool *flag, int check)
{
    static bool n;

    if (check)
    {
        n = *flag;
    }
    else
    {
        *flag = n;
    }
}

void get_sockid(int *flag, int check)
{
    static int n;

    if (check)
    {
        n = *flag;
    }
    else
    {
        *flag = n;
    }
}

void get_login(char **login, int check)
{
    static char *n;

    if (check)
    {
        n = *login;
    }
    else
    {
        *login = n;
    }
}

void get_password(char **password, int check)
{
    static char *n;

    if (check)
    {
        n = *password;
    }
    else
    {
        *password = n;
    }
}

void get_array(char **array, int check)
{ // to an array from main
    static char *n;

    if (check)
    {
        n = *array;
    }
    else
    {
        *array = n;
    }
}
