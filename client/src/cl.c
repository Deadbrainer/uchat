#include "../inc/client.h"

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    if (argc != 2)
    {
        fprintf(stderr, "Wrong usage\n");
        return 0;
    }

    int port = mx_atoi(argv[1]);

    char *msg = malloc(500);
    int sock;

    struct sockaddr_in *ServerIp = malloc(sizeof(struct sockaddr_in));

    sock = socket(AF_INET, SOCK_STREAM, 0);
    ServerIp->sin_family = AF_INET;
    ServerIp->sin_port = htons(port);
    ServerIp->sin_addr.s_addr = inet_addr("127.0.0.1");
    sock = socket(AF_INET, SOCK_STREAM, 0);

    get_struct_socaddr(&ServerIp, 1);
    if (connect(sock, (struct sockaddr *)ServerIp, sizeof(*ServerIp)) == -1)
    {
        fprintf(stderr, "Connection failure\n");
        return 0;
    }
    else
    {
        printf("Connected\n");
    }

    get_array(&msg, 1); // assign value
    get_sockid(&sock, 1);

    login_menu();

    // pid = getpid();

    // signal(SIGUSR1, handle);

    gtk_main();
    close(sock);
    return 0;
}
