#include "../inc/client.h"

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
        n = mx_strdup(*login);
    }
    else
    {
        *login = mx_strdup(n);
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

void get_list(t_list **x, int check)
{
    static t_list *n;

    if (check)
    {
        n = *x;
    }
    else
    {
        *x = n;
    }
}

char *get_date()
{
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    char *res = mx_itoa(timeinfo->tm_hour);
    res = mx_strjoin(res, ":");
    res = mx_strjoin(res, mx_itoa(timeinfo->tm_min));
    return res;
}

void get_struct_socaddr(struct sockaddr_in **SocketIp, int check) {
    static struct sockaddr_in *n;
    if (check)
    {
        n = *SocketIp;
    }
    else
    {
        *SocketIp = n;
    }
}