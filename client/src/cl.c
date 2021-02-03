
#include "../inc/uchat.h"

// char msg[500];

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

void get_sockid(int *sockid, int check)
{ // to an array from main
    static int n;

    if (check)
    {
        n = *sockid;
    }
    else
    {
        *sockid = n;
    }
}

void *recvmg(void *my_sock)
{
    printf("Waintig for messages to write\n");
    char *msg;
    get_array(&msg, 0);

    int sock = *((int *)my_sock);
    int len;
    while ((len = recv(sock, msg, 500, 0)) > 0)
    {
        msg[len] = '\0';
        fputs(msg, stdout);
    }
    return NULL; // to silence warning
} // should not return anything

void str_trim_lf(char *arr, int length)
{
    int i;
    for (i = 0; i < length; i++)
    { // trim \n
        if (arr[i] == '\n')
        {
            arr[i] = '\0';
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    if (argc != 2)
    {
        fprintf(stderr, "Wrong usage\n");
        return 0;
    }

    int port = mx_atoi(argv[1]);

    //char msg[500];
    char *msg = malloc(500);

    pthread_t recvt;
    int len;
    int sock;

    //char *send_msg = malloc(500);

    struct sockaddr_in ServerIp;

    // strcpy(client_name, argv[1]);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    ServerIp.sin_family = AF_INET;
    ServerIp.sin_port = htons(port);
    ServerIp.sin_addr.s_addr = inet_addr("127.0.0.1");
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(sock, (struct sockaddr *)&ServerIp, sizeof(ServerIp)) == -1)
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

    login_menu(false);
    gtk_main();

    pthread_create(&recvt, NULL, (void *)recvmg, &sock); // client thread which is always waiting for a message
    // gtk windows

    //read a message from stdin (console)
    while (fgets(msg, 500, stdin) > 0)
    {
        // strcpy(send_msg, client_name);
        // strcat(send_msg, ":  ");
        // strcat(send_msg, msg);
        len = write(sock, msg, strlen(msg));
        if (len < 0)
        {
            printf("\n message not send");
        }
    }

    //thread is closed
    pthread_join(recvt, NULL);
    close(sock);
    return 0;
}
