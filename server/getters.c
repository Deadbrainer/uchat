#include "server.h"

void getDataBase(sqlite3 *x, int check)
{ // to get variables from main
    static sqlite3 *n;

    if (check)
    {
        n = x;
    }
    else
    {
        x = n;
    }
}

void get_password(char **password, int check)
{ // to get variables from main
    static char *n;

    if (check)
    {
        n = mx_strdup(*password);
    }
    else
    {
        *password = mx_strdup(n);
    }
}

void get_mutex(pthread_mutex_t *x, int check)
{ // to get variables from main
    static pthread_mutex_t n;

    if (check)
    {
        n = *x;
    }
    else
    {
        *x = n;
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
