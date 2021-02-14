#include "server.h"

void *recvmg(void *client_sock)
{ // that func can only accept void* argument
    int sock = *((int *)client_sock);
    char *msg = mx_strnew(500);
    int len;
    char *send_msg = malloc(500);
    char *name = NULL;
    char *password_test = NULL;
    char *password = NULL;
    char **cut_str = NULL;
    char **splited_msg = NULL;
    bool check_r = false;

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
            if (msg[5] == '\r')
            {
                cut_str = mx_strsplit(msg, '\v');
                name = mx_strdup(cut_str[1]);
                add_sockid_into_user(db, name, sock);
            }
            else
            {
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
                name = mx_strdup(cut_str[1]);
                password = mx_strdup(cut_str[2]);
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
                    else if (check_signin == 0)
                    {
                        check_r = true;
                    }
                }
                password_test = get_password_from_db(db, name);
                if (password_test && (strcmp(cut_str[2], password_test) == 0))
                {
                    send_back[1] = 'Y';
                }
                else
                {
                    send_back[1] = 'N';

                    if (check_signin == 1)
                    {
                        count_to_2 = -1;
                    }
                }

                //! SENDING ANSWER

                if (send(sock, send_back, 3, 0) < 0)
                {
                    fprintf(stderr, "sending failure\n");
                }

                if ((check_signin == 1 && mx_strcmp("YY", send_back) == 0))
                {
                    get_from_db_users(db);
                    get_rooms_from_db(db);
                    add_sockid_into_user(db, name, sock);
                    char *roomname = NULL;
                    char **id_rooms = mx_strsplit(get_idrooms_from_users(db, name), '\v');
                    while (*id_rooms != NULL)
                    {
                        if (mx_strcmp(*id_rooms, "0"))
                        {
                            roomname = get_roomnames_from_rooms(db, *id_rooms);
                            roomname = mx_strjoin("\r\v", roomname);
                            roomname = mx_strjoin(roomname, "\t"); // splitim po \t
                            roomname = mx_strjoin(roomname, *id_rooms);
                            send(sock, roomname, mx_strlen(roomname), 0);
                        }
                        id_rooms++;
                    }
                }
                if (check_signin == 0 && check_r == true)
                {
                    insert_into_db_users(db, name, password, sock);
                }
                mx_del_strarr(&cut_str);
            }
            count_to_2++;
            break;
        default:

            splited_msg = mx_strsplit(msg, '\v');

            if (msg[0] == '\r' && msg[1] != '\r') // create room spliting using \v
            {
                if (get_roomnames_from_rooms_by_roomname(db, splited_msg[1]) == NULL)
                {
                    insert_into_db_room(db, splited_msg[1], name);
                    int id_created = get_roomid_from_room_with_roomname(db, splited_msg[1]);
                    add_idroom_into_user(db, name, id_created);
                    send(sock, "Y", mx_strlen("Y"), 0); // Y roomname does not exist
                }
                else
                {
                    send(sock, "N", mx_strlen("N"), 0); // N roomname existed
                }
            }
            else if (msg[1] == '\r' && msg[2] != '\r') // add user to the room (1 = Username, 2 = roomname)
            {
                bool check_add = false;
                if (get_roomnames_from_rooms_by_roomname(db, splited_msg[2]) == NULL)
                {
                    send(sock, "N", mx_strlen("N"), 0); // NN netu rooma
                    break;
                }
                for (t_list *a = users; a != NULL; a = a->next)
                {

                    if (mx_strcmp(splited_msg[1], a->data) == 0)
                    {
                        bool check_id_if_exist = false;
                        char **idrooms = mx_strsplit(get_idrooms_from_users(db, splited_msg[1]), '\v');
                        while (*idrooms != NULL)
                        {
                            if (mx_strcmp(mx_itoa(get_roomid_from_room_with_roomname(db, splited_msg[2])), *idrooms) == 0)
                            {
                                send(sock, "NN", mx_strlen("NN"), 0); // 2 NN etot user uzhe priglashon
                                check_id_if_exist = true;
                                break;
                            }
                            idrooms++;
                        }
                        if (check_id_if_exist == false)
                        {
                            add_idroom_into_user(db, splited_msg[1], get_roomid_from_room_with_roomname(db, splited_msg[2]));
                            add_user_into_room(db, splited_msg[1], mx_itoa(get_roomid_from_room_with_roomname(db, splited_msg[2])));
                            send(get_sockid_from_db(db, splited_msg[1]), mx_strjoin("\r\r\v", splited_msg[2]), mx_strlen(mx_strjoin("\r\r\v", splited_msg[2])), 0); // Send to user who has been added
                            check_add = true;
                            break;
                        }
                    }
                }
                if (check_add == false)
                {
                    send(sock, "NNN", mx_strlen("NNN"), 0); // 1 N = netu takovo usera
                }
                else
                {
                    send(sock, "Y", mx_strlen("Y"), 0); // Y vse okey priglos poshel
                }
            }
            else if (msg[2] == '\r' && msg[3] != '\r') // get messages from room (1 = room_name, 2 = text)
            {
                insert_into_db_message(db, mx_itoa(get_roomid_from_room_with_roomname(db, splited_msg[1])), name, splited_msg[2]);
                char *date = get_date_from_message(db, splited_msg[2]);
                char *send_users = get_usernames_from_rooms(db, mx_itoa(get_roomid_from_room_with_roomname(db, splited_msg[1])));
                char **splited_users = mx_strsplit(send_users, '\v');
                while (*splited_users != NULL)
                {
                    send_msg = mx_strjoin("\r\r\r\v", splited_msg[1]);
                    send_msg = mx_strjoin(send_msg, "\v");
                    send_msg = mx_strjoin(send_msg, name);
                    send_msg = mx_strjoin(send_msg, ":  ");
                    send_msg = mx_strjoin(send_msg, splited_msg[2]);
                    send_msg = mx_strjoin(send_msg, " ");
                    send_msg = mx_strjoin(send_msg, date);
                    if (sock != get_sockid_from_db(db, *splited_users))
                    {
                        send(get_sockid_from_db(db, *splited_users), send_msg, mx_strlen(send_msg), 0);
                    }
                    splited_users++;
                }
            }
            else if (msg[3] == '\r' && msg[4] != '\r')
            {
                add_sockid_into_user(db, name, 0);
            }
            else if (msg[4] == '\r' && msg[5] != '\r') // show message 1 = room_name
            {
                printf("%s\n", splited_msg[1]);
                if (splited_msg[1] != NULL)
                {
                    char **message_text = get_text_from_message(db, get_roomid_from_room_with_roomname(db, splited_msg[1]));
                    char *send_users = get_usernames_from_rooms(db, mx_itoa(get_roomid_from_room_with_roomname(db, splited_msg[1])));
                    char **splited_users = mx_strsplit(send_users, '\v');
                    while (*splited_users != NULL && *message_text != NULL)
                    {
                        char *date = get_date_from_message(db, *message_text);
                        send_msg = mx_strjoin("\r\r\r\r\v", splited_msg[1]);
                        send_msg = mx_strjoin(send_msg, "\v");
                        send_msg = mx_strjoin(send_msg, name);
                        send_msg = mx_strjoin(send_msg, ":  ");
                        send_msg = mx_strjoin(send_msg, *message_text);
                        send_msg = mx_strjoin(send_msg, " ");
                        send_msg = mx_strjoin(send_msg, date);
                        if (sock != get_sockid_from_db(db, *splited_users))
                        {
                            send(get_sockid_from_db(db, *splited_users), send_msg, mx_strlen(send_msg), 0);
                        }
                        splited_users++;
                        message_text++;
                    }
                }
            }

            get_from_db_users(db);
            get_rooms_from_db(db);
            bzero(msg, mx_strlen(msg)); //! NEW
        }
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
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
    {
        fprintf(stderr, "setsockopt error\n");
    }
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
