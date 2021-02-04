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

void get_list(t_list** x, int check) {
    static t_list* n;

    if(check) {
        n = *x;
    } else {
        *x = n;
    }
}

void send_everyone(char *msg, int sock, pthread_mutex_t *mutex_GLOBAL, t_list** ids)
{
    printf("Called me (send everyone)\n");
    pthread_mutex_lock(mutex_GLOBAL); // so that different threads didnt do that at one time (might crash)

    // for (int i = 0; i < *n_GLOBAL; i++)
    // {
    //     if (clients_GLOBAL[i] != sock)
    //     {
    //         if (send(clients_GLOBAL[i], msg, strlen(msg), 0) < 0)
    //         {
    //             fprintf(stderr, "sending failure\n");
    //         }
    //     }
    // }

    for (t_list* a = *ids; a != NULL; a = a->next) {
        if (mx_atoi((char*)(a->data)) != sock) {
            if (send(mx_atoi((char*)(a->data)), msg, strlen(msg), 0) < 0) {
                fprintf(stderr, "sending failure\n");
            }
        }
    }

    pthread_mutex_unlock(mutex_GLOBAL);
}

void *recvmg(void *client_sock)
{                                     // that func can only accept void* argument
    int sock = *((int *)client_sock); // so that is happening))
    char msg[500];
    int len;
    char *send_msg;
    //char send_msg[500];
    char *name = NULL;

    // static int n_GLOBAL;
    // get_N(&n_GLOBAL, 0);

    static pthread_mutex_t mutex_GLOBAL;
    get_mutex(&mutex_GLOBAL, 0);

    // static int clients_GLOBAL[20];

    // get_arr(clients_GLOBAL, 0);

    t_list* ids = NULL;
    get_list(&ids, 0);

    // printf("Crash here?\n");

    // for (t_list* a = ids; a != NULL; a = a->next) {
    //     printf("%s\n", a->data);
    // }

    // printf("Nope\n");

    static sqlite3 *db;
    getDataBase(db, 0);

    int count_to_2 = 0;
    char **get;

    char* sen = mx_strnew(1);

    while ((len = recv(sock, msg, 500, 0)) > 0)
    {
        msg[len] = '\0';

        if (count_to_2 == 0)
        {
            get = mx_strsplit(msg, ' ');
        }
        switch (count_to_2)
        {
        case 0:
            name = get[0];
            //get_from_db_users(db);

            t_list *users = get_usernames_from_db(db);

            int error = 0;

            // bool e;

            for (t_list* a = users; a != NULL; a = a->next) {
                if (mx_strcmp(get[0], a->data) == 0) {

                    sen[0] = 'Y';

                    printf("sENDING YES\n");

                    if (send(sock, sen, 2, 0) < 0)
                    {
                        fprintf(stderr, "sending failure\n");
                    }
                    count_to_2--;
                    error = 1;
                    break;
                }
            }
            if (!error) {
                insert_into_db_users(db, get[0], get[1]);

                sen[0] = 'N';

                printf("sENDING NO\n");

                if (send(sock, sen, 2, 0) < 0)
                {
                    fprintf(stderr, "sending failure\n");
                }
            }
            
            break;
        default:
            //insert_into_db_message(db, name, msg);
            //get_from_db_messages(db);

            printf("On the server: %s\n", msg);

            send_msg = mx_strjoin(name, ":  ");
            send_msg = mx_strjoin(send_msg, msg);

            printf("On the server send_msg: %s\n", send_msg);

            // strcpy(send_msg, name);
            // strcat(send_msg, ":  ");
            // strcat(send_msg, msg);
            //send_everyone(send_msg, sock, &mutex_GLOBAL, clients_GLOBAL, &n_GLOBAL);
            send_everyone(send_msg, sock, &mutex_GLOBAL, &ids);

            //memset(msg, strlen(msg), '\0'); //! NEW
        }
        memset(msg, strlen(msg), '\0');
        memset(send_msg, strlen(send_msg), '\0');
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
    //static int clients_GLOBAL[20];

    // for (int i = 0; i < 20; i++)
    // {
    //     clients_GLOBAL[i] = 0;
    // }
    //static t_list* clients_GLOBAL = NULL;
    //static int n_GLOBAL = 0;

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
        return 0;
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


    t_list *ids = NULL;


    pid_t pid;
    pid = getpid();
    printf("Process ID is: %d\n", pid); //! PID Printing


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
        //clients_GLOBAL[n_GLOBAL] = Client_sock;

        mx_push_back(&ids, mx_strdup(mx_itoa(Client_sock)));

        // for (t_list* i = clients_GLOBAL; i != NULL; i = i->next) {
        //     mx_printstr("DATA: ");
        //     mx_printint(*((int*)i->data));
        //     mx_printstr("\n");
        // }

        //mx_static_push_back(&clients_GLOBAL, &Client_sock);
        //n_GLOBAL++;

        //get_N(&n_GLOBAL, 1);         // to give a value
        get_mutex(&mutex_GLOBAL, 1); // to give a value
        //get_arr(clients_GLOBAL, 1);  // to give a value
        get_list(&ids, 1);
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
