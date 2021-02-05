#include "server.h"

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
    char **cut_str = NULL;

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
        msg[len] = '\0';

        switch (count_to_2)
        {
        case 0:
            cut_str = mx_strsplit(msg, ' ');

            //! CHECKING IF LOG OR REG

            if (mx_strcmp(cut_str[0], "L") == 0)
            {
                check_signin = 1;
                bzero(msg, 1);
            }
            else if (mx_strcmp(cut_str[0], "R") == 0)
            {
                check_signin = 0;
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
                    if (check_signin == 0)
                    {
                        count_to_2 = -1;
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

                send_back[0] = 'N';
                if (check_signin == 1)
                {
                    count_to_2 = -1;
                }
            }

            password = get_password_from_db(db, name);

            if (strcmp(cut_str[2], password) == 0)
            {

                send_back[1] = 'Y';
            }
            else
            {

                send_back[1] = 'N';
                count_to_2 = -1;
            }

            //! SENDING ANSWER

            if (send(sock, send_back, 3, 0) < 0)
            {
                fprintf(stderr, "sending failure\n");
            }
            mx_del_strarr(&cut_str);
            break;
        default:

            if (check_signin == 0)
            {
                get_password(&password, 0);
                insert_into_db_users(db, name, password);
                get_from_db_users(db);
            }

            //insert_into_db_message(db, name, msg);
            //get_from_db_messages(db);
            strcpy(send_msg, name);
            strcat(send_msg, ":  ");
            strcat(send_msg, msg);
            send_everyone(send_msg, sock, &mutex_GLOBAL, &ids);

            memset(msg, strlen(msg), '\0'); //! NEW
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
    //get_from_db_users(db);

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
