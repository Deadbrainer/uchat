
#include "../inc/client.h"

void closeApp(GtkWidget *window, gpointer data)
{
    GtkWidget *a = window;
    a = NULL;
    
    data = NULL;
    int sock = 0;
    get_sockid(&sock, 0);
    send(sock, "\r\r\r\r\vclose", mx_strlen("\r\r\r\r\vclose"), 0);
    gtk_main_quit();
}
