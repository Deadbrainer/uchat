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

// void get_username(char **name, int check)
// { // to get variables from main
//     static char *n;

//     if (check)
//     {
//         n = mx_strdup(*name);
//     }
//     else
//     {
//         *name = mx_strdup(n);
//     }
// }

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

void send_everyone(char *msg, int sock, pthread_mutex_t *mutex_GLOBAL, t_list **ids)
{
    printf("Called me (send everyone)\n");
    pthread_mutex_lock(mutex_GLOBAL); // so that different threads didnt do that at one time (might crash)

    for (t_list *a = *ids; a != NULL; a = a->next)
    {
        if (mx_atoi((char *)(a->data)) != sock)
        {
            if (send(mx_atoi((char *)(a->data)), msg, strlen(msg), 0) < 0)
            {
                fprintf(stderr, "sending failure\n");
            }
        }
    }

    pthread_mutex_unlock(mutex_GLOBAL);
}

void *recvmg(void *client_sock)
{ // that func can only accept void* argument
    int sock = *((int *)client_sock);
    char *msg = mx_strnew(500);
    int len;
    char *send_msg = malloc(500);
    char *name = NULL;
    char *password = NULL;

    static pthread_mutex_t mutex_GLOBAL;
    get_mutex(&mutex_GLOBAL, 0);

    t_list *ids = NULL;
    get_list(&ids, 0);
    static sqlite3 *db;
    getDataBase(db, 0);
    int check_signin = 0;

    int count_to_2 = 0;
    t_list *users = NULL;


    char send_back[3];
    send_back[2] = '\0';

    while ((len = recv(sock, msg, 500, 0)) > 0)
    {


        printf("Got message: %s\n", msg);


        msg[len] = '\0';

        switch (count_to_2)
        {
        case 0:

            // printf("checking if reg or log\n");

            // if (mx_strcmp(msg, "L\0") == 0)
            // {
            //     check_signin = 1;
            //     memset(msg, 1, '\0');
            // }

            printf("checking string\n");

            char** cut_str = mx_strsplit(msg, ' ');

            //! CHECKING IF LOG OR REG

            if (mx_strcmp(cut_str[0], "L") == 0) {
                check_signin = 1;
                bzero(msg, 1);
            }

            char send_back[3];
            send_back[2] = '\0';

            //! PROCESSING LOGIN

            users = get_usernames_from_db(db);
            int error = 0;
            for (t_list *a = users; a != NULL; a = a->next)
            {
                if (mx_strcmp(cut_str[1], a->data) == 0)
                {

                    send_back[0] = 'Y';

                    printf("Found such name\n");

                    // if (send(sock, sen, 2, 0) < 0)
                    // {
                    //     fprintf(stderr, "sending failure\n");
                    // }
                    if (check_signin == 0)
                    {
                        count_to_2 = 0;
                    }
                    else
                    {
                        name = mx_strdup(cut_str[1]);
                    }
                    error = 1;
                    break;
                }
            }
            if (!error)
            {

                printf("Didnt found ound such name\n");

                send_back[0] = 'N';

                //printf("sENDING NO\n");
                if (check_signin == 1)
                {
                    count_to_2 = -1;
                }
                // if (send(sock, sen, 2, 0) < 0)
                // {
                //     fprintf(stderr, "sending failure\n");
                // }
            }

            //!PROCESSING PASSWORD

            password = get_password_from_db(db, name);

            if (strcmp(cut_str[2], password) == 0)
            {
                printf("Password matches\n");

                send_back[1] = 'Y';

                //printf("sENDING YES\n");

                // if (send(sock, sen, 2, 0) < 0)
                // {
                //     fprintf(stderr, "sending failure\n");
                // }
                // get_password(&msg, 1);
            }
            else
            {
                printf("Password doesnt match\n");

                printf("Entered password is: %s\n", cut_str[2]);
                printf("Real password is: %s\n", password);

                send_back[1] = 'N';

                // printf("sENDING NO\n");

                // if (send(sock, sen, 2, 0) < 0)
                // {
                //     fprintf(stderr, "sending failure\n");
                // }
                count_to_2 = -1;
            }

            //! SENDING ANSWER

            printf("sENDING answer: %s\n", send_back);

            if (send(sock, send_back, 3, 0) < 0)
            {
                fprintf(stderr, "sending failure\n");
            }
            break;
        // case 1:

        //     printf("checking username\n");

        //     //get_from_db_users(db);
        //     users = get_usernames_from_db(db);
        //     int error = 0;
        //     for (t_list *a = users; a != NULL; a = a->next)
        //     {
        //         if (mx_strcmp(msg, a->data) == 0)
        //         {

        //             sen[0] = 'Y';

        //             printf("sENDING YES\n");

        //             if (send(sock, sen, 2, 0) < 0)
        //             {
        //                 fprintf(stderr, "sending failure\n");
        //             }
        //             if (check_signin == 0)
        //             {
        //                 count_to_2 = 0;
        //             }
        //             else
        //             {
        //                 name = mx_strdup(msg);
        //             }
        //             error = 1;
        //             break;
        //         }
        //     }
        //     if (!error)
        //     {

        //         sen[0] = 'N';

        //         printf("sENDING NO\n");
        //         if (check_signin == 1)
        //         {
        //             count_to_2 = 0;
        //         }
        //         if (send(sock, sen, 2, 0) < 0)
        //         {
        //             fprintf(stderr, "sending failure\n");
        //         }
        //     }
        //     break;
        // case 2:

        //     printf("checking password\n");

        //     password = get_password_from_db(db, name);
        //     if (mx_strcmp(msg, password) == 0)
        //     {
        //         sen[0] = 'Y';

        //         printf("sENDING YES\n");

        //         if (send(sock, sen, 2, 0) < 0)
        //         {
        //             fprintf(stderr, "sending failure\n");
        //         }
        //         get_password(&msg, 1);
        //         error = 1;
        //         break;
        //     }
        //     else
        //     {

        //         sen[0] = 'N';

        //         printf("sENDING NO\n");

        //         if (send(sock, sen, 2, 0) < 0)
        //         {
        //             fprintf(stderr, "sending failure\n");
        //         }
        //         count_to_2 = 0;
        //     }
        //     break;
        default:

            printf("receiving message\n");


            if (check_signin == 0)
            {
                get_password(&password, 0);
                insert_into_db_users(db, name, password);
            }

            //insert_into_db_message(db, name, msg);
            //get_from_db_messages(db);
            strcpy(send_msg, name);
            strcat(send_msg, ":  ");
            strcat(send_msg, msg);
            //send_everyone(send_msg, sock, &mutex_GLOBAL, clients_GLOBAL, &n_GLOBAL);
            send_everyone(send_msg, sock, &mutex_GLOBAL, &ids);

            memset(msg, strlen(msg), '\0'); //! NEW
        }
        //memset(msg, strlen(msg), '\0');
        count_to_2++;

        printf("Now count is: %i\n", count_to_2);

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
    get_from_db_users(db);

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
        pthread_mutex_lock(&mutex_GLOBAL);

        mx_push_back(&ids, mx_strdup(mx_itoa(Client_sock)));
        get_mutex(&mutex_GLOBAL, 1); // to give a value

        get_list(&ids, 1);
        getDataBase(db, 1); // to give a value

        pthread_create(&recvt, NULL, (void *)recvmg, &Client_sock);
        pthread_mutex_unlock(&mutex_GLOBAL);
    }
    return 0;
}
