#include "../inc/client.h"

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
            sleep(1);
        }
        else
        {
            get_sockid(&sock, 1);

            char *name;
            get_login(&name, 0);
            send(sock, mx_strjoin("\r\r\r\r\r\r\v", name), strlen(mx_strjoin("\r\r\r\r\r\r\v", name)), 0);
            recvmg_new(&sock);
            break;
        }
    }
}

void try_reconnect_login()
{
    struct sockaddr_in *ServerIp = NULL;
    get_struct_socaddr(&ServerIp, 0);

    int sock;

    while (1)
    {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (connect(sock, (struct sockaddr *)ServerIp, sizeof(*ServerIp)) == -1)
        {
            sleep(1);
        }
        else
        {
            get_sockid(&sock, 1);

            char *name;
            get_login(&name, 0);
            send(sock, mx_strjoin("\r\r\r\r\r\r\v", name), strlen(mx_strjoin("\r\r\r\r\r\r\v", name)), 0);
            break;
        }
    }
}
