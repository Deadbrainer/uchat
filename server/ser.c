#include "server.h"

// pthread_mutex_t mutex_GLOBAL;
//int clients_GLOBAL[20];
// int n_GLOBAL = 0;

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

void get_N(int *x, int check)
{ // to get variables from main
    static int n;

    if (check)
    {
        n = *x;
    }
    else
    {
        *x = n;
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

void get_arr(int d[], int check)
{ // to an array from main
    static int arr[20];

    if (check)
    {
        for (int i = 0; i < 20; i++)
        {
            arr[i] = d[i];
        }
    }
    else
    {
        for (int i = 0; i < 20; i++)
        {
            d[i] = arr[i];
        }
    }
}

typedef struct f
{
    char *name_password;
} client;

// static client* get_struct(client** x, int check) {
//     static client* n = NULL;

//     if(check) {
//         n = *x;
//     } else {
//         *x = n;
//     }

//     return n;
// }

void send_everyone(char *msg, int sock, pthread_mutex_t *mutex_GLOBAL, int clients_GLOBAL[], int *n_GLOBAL)
{
    // int i;

    pthread_mutex_lock(mutex_GLOBAL); // so that different threads didnt do that at one time (might crash)

    // mx_printstr("The quantity of clients is: ");
    // mx_printint(*n_GLOBAL);
    // mx_printstr("\n");

    for (int i = 0; i < *n_GLOBAL; i++)
    {
        if (clients_GLOBAL[i] != sock)
        {
            if (send(clients_GLOBAL[i], msg, strlen(msg), 0) < 0)
            {
                fprintf(stderr, "sending failure\n");
            }
        }
    }

    // for (t_list* i = *clients_GLOBAL; i != NULL; i = i->next) {

    //     mx_printstr("SockId is: ");
    //     mx_printint(*((int*)i->data));
    //     mx_printstr("\n");

    //     if (*((int*)i->data) != sock) {
    //         if (send(*((int*)i->data), msg, strlen(msg), 0) < 0) {
    //             fprintf(stderr, "sending failure\n");
    //         }
    //     }
    // }
    pthread_mutex_unlock(mutex_GLOBAL);
}

void *recvmg(void *client_sock)
{                                     // that func can only accept void* argument
    int sock = *((int *)client_sock); // so that is happening))
    char msg[500];
    int len;

    static int n_GLOBAL;
    get_N(&n_GLOBAL, 0);

    static pthread_mutex_t mutex_GLOBAL;
    get_mutex(&mutex_GLOBAL, 0);

    static int clients_GLOBAL[20];

    //static t_list* clients_GLOBAL = NULL;
    get_arr(clients_GLOBAL, 0);
    //clients_GLOBAL = get_list(0, 0);

    static sqlite3 *db;
    getDataBase(db, 0);

    int count_to_2 = 0;

    // client* R = malloc(sizeof(client));

    while ((len = recv(sock, msg, 500, 0)) > 0)
    {
        //mx_printstr("GOT MESSAGE\n");
        msg[len] = '\0';

        char **get;

        switch (count_to_2)
        {
        case 0:
            //R->name_password = mx_strdup(msg);
            get = mx_strsplit(msg, ' ');

            // mx_printstr("Name: ");
            // mx_printstr(get[0]);
            // mx_printstr("\n");

            // mx_printstr("Password: ");
            // mx_printstr(get[1]);
            // mx_printstr("\n");

            insert_into_db(db, get[0], get[1]);

            get_from_db_users(db);
            //get_struct(&R, 1);
        default:
            send_everyone(msg, sock, &mutex_GLOBAL, clients_GLOBAL, &n_GLOBAL);
        }

        count_to_2++;
    }

    return NULL; // to silence warning
} // should not return anything

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        fprintf(stderr, "Wrong usage\n");
        return 0;
    }

    int port = mx_atoi(argv[1]);

    static pthread_mutex_t mutex_GLOBAL;
    static int clients_GLOBAL[20];

    for (int i = 0; i < 20; i++)
    {
        clients_GLOBAL[i] = 0;
    }
    //static t_list* clients_GLOBAL = NULL;
    static int n_GLOBAL = 0;

    struct sockaddr_in ServerIp;
    pthread_t recvt;
    int sock = 0;
    int Client_sock = 0;
    ServerIp.sin_family = AF_INET;
    ServerIp.sin_port = htons(port);
    ServerIp.sin_addr.s_addr = inet_addr("127.0.0.1");
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (bind(sock, (struct sockaddr *)&ServerIp, sizeof(ServerIp)) == -1)
    {
        fprintf(stderr, "binding failure\n");
    }
    else
    {
        printf("Server started\n");
    }

    if (listen(sock, 20) == -1)
    {
        fprintf(stderr, "listening failure\n");
    }

    static sqlite3 *db; // CREATE DATABASE
    create_table(db);

    while (1)
    {
        if ((Client_sock = accept(sock, (struct sockaddr *)NULL, NULL)) < 0)
        {
            fprintf(stderr, "accepting failure\n");
        }
        // mx_printstr("CREATED SOCKET IS: ");
        // mx_printint(Client_sock);
        // mx_printstr("\n");

        pthread_mutex_lock(&mutex_GLOBAL);
        clients_GLOBAL[n_GLOBAL] = Client_sock;

        // mx_push_back(&clients_GLOBAL, &Client_sock);
        // for (t_list* i = clients_GLOBAL; i != NULL; i = i->next) {
        //     mx_printstr("DATA: ");
        //     mx_printint(*((int*)i->data));
        //     mx_printstr("\n");
        // }

        //mx_static_push_back(&clients_GLOBAL, &Client_sock);
        n_GLOBAL++;

        get_N(&n_GLOBAL, 1);         // to give a value
        get_mutex(&mutex_GLOBAL, 1); // to give a value
        get_arr(clients_GLOBAL, 1);  // to give a value
        getDataBase(db, 1);          // to give a value

        //get_list(&clients_GLOBAL, 1);
        //get_list(&Client_sock, 1);

        pthread_create(&recvt, NULL, (void *)recvmg, &Client_sock);
        pthread_mutex_unlock(&mutex_GLOBAL);

        //client* A = NULL;

        // while(1) {
        //     get_struct(&A, 0);
        //     if (A) {
        //         mx_printstr("Got data\n");

        //         char** a = mx_strsplit(A->name_password, ' ');

        //         mx_printstr("A name is: ");
        //         mx_printstr(a[0]);
        //         mx_printstr("\n");

        //         mx_printstr("A password is: ");
        //         mx_printstr(a[1]);
        //         mx_printstr("\n");

        //         break;
        //     }
        // }
    }
    return 0;
}
