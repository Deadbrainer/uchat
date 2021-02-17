#include "../inc/server.h"

// Get ID rooms
char *get_idrooms_from_users(sqlite3 *db, char *username)
{
    sqlite3_stmt *res;
    char *a = NULL;

    int rc = sqlite3_open("uchat.db", &db);

    if (rc != SQLITE_OK)
    {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return NULL;
    }

    rc = sqlite3_prepare_v2(db, "SELECT ID_ROOMS FROM USERS WHERE NAME = ?", -1, &res, 0);
    sqlite3_bind_text(res, 1, username, -1, SQLITE_STATIC);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error int searching: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }
    rc = sqlite3_step(res);
    a = mx_strdup((char *)sqlite3_column_text(res, 0));
    sqlite3_finalize(res);
    sqlite3_close(db);

    return a;
}

// Get Roomnames
char *get_roomnames_from_rooms(sqlite3 *db, char *id)
{
    sqlite3_stmt *res;
    char *room_name = NULL;

    int rc = sqlite3_open("uchat.db", &db);

    if (rc != SQLITE_OK)
    {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return NULL;
    }

    rc = sqlite3_prepare_v2(db, "SELECT NAME FROM ROOMS WHERE ID = ?", -1, &res, 0);
    sqlite3_bind_text(res, 1, id, -1, SQLITE_STATIC);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error int searching: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }

    rc = sqlite3_step(res);
    if (rc == SQLITE_ROW)
    {
        room_name = mx_strdup((char *)sqlite3_column_text(res, 0));
    }
    sqlite3_finalize(res);
    sqlite3_close(db);

    return room_name;
}

char *get_roomnames_from_rooms_by_roomname(sqlite3 *db, char *roomname)
{
    sqlite3_stmt *res;
    char *room_name = NULL;

    int rc = sqlite3_open("uchat.db", &db);

    if (rc != SQLITE_OK)
    {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return NULL;
    }

    rc = sqlite3_prepare_v2(db, "SELECT USERNAMES FROM ROOMS WHERE NAME = ?", -1, &res, 0);
    sqlite3_bind_text(res, 1, roomname, -1, SQLITE_STATIC);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error int searching: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }

    rc = sqlite3_step(res);
    if (rc == SQLITE_ROW)
    {
        room_name = mx_strdup((char *)sqlite3_column_text(res, 0));
    }
    sqlite3_finalize(res);
    sqlite3_close(db);

    return room_name;
}
// Get users from rooms
char *get_usernames_from_rooms(sqlite3 *db, char *id)
{
    sqlite3_stmt *res;
    char *user_name;

    int rc = sqlite3_open("uchat.db", &db);

    if (rc != SQLITE_OK)
    {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return NULL;
    }

    rc = sqlite3_prepare_v2(db, "SELECT USERNAMES FROM ROOMS WHERE ID = ?", -1, &res, 0);
    sqlite3_bind_text(res, 1, id, -1, SQLITE_STATIC);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error int searching: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }
    rc = sqlite3_step(res);
    user_name = mx_strdup((char *)sqlite3_column_text(res, 0));

    sqlite3_finalize(res);
    sqlite3_close(db);

    return user_name;
}

// Get password
char *get_password_from_db(sqlite3 *db, char *name)
{
    sqlite3_stmt *restt;
    char *password = NULL;
    int rc = sqlite3_open("uchat.db", &db);

    if (rc != SQLITE_OK)
    {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return NULL;
    }

    rc = sqlite3_prepare_v2(db, "SELECT PASSWORD FROM USERS WHERE NAME = ?", -1, &restt, 0);
    sqlite3_bind_text(restt, 1, name, -1, SQLITE_STATIC);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error int searching: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }
    rc = sqlite3_step(restt);
    if (rc == SQLITE_ROW)
    {
        password = mx_strdup((char *)sqlite3_column_text(restt, 0));
    }

    sqlite3_finalize(restt);
    sqlite3_close(db);

    return password;
}

// Get usernames
t_list *get_usernames_from_db(sqlite3 *db)
{
    sqlite3_stmt *rest;

    int rc = sqlite3_open("uchat.db", &db);

    if (rc != SQLITE_OK)
    {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return NULL;
    }

    char *sql = "SELECT name FROM USERS";
    rc = sqlite3_prepare_v2(db, sql, -1, &rest, 0);

    t_list *ss = NULL;
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error int searching: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }
    while (sqlite3_step(rest) == SQLITE_ROW)
    {
        char *a = (char *)sqlite3_column_text(rest, 0);

        mx_push_back(&ss, strdup(a));
    }

    sqlite3_finalize(rest);
    sqlite3_close(db);

    return ss;
}

// Get sockid
int get_sockid_from_db(sqlite3 *db, char *name)
{
    sqlite3_stmt *res;
    int sockid;

    int rc = sqlite3_open("uchat.db", &db);

    if (rc != SQLITE_OK)
    {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return 0;
    }

    rc = sqlite3_prepare_v2(db, "SELECT SOCK_ID FROM USERS WHERE NAME = ?", -1, &res, 0);
    sqlite3_bind_text(res, 1, name, -1, SQLITE_STATIC);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error int searching: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }
    rc = sqlite3_step(res);
    sockid = sqlite3_column_int(res, 0);
    sqlite3_finalize(res);
    sqlite3_close(db);

    return sockid;
}

int get_roomid_from_room_with_roomname(sqlite3 *db, char *name)
{
    sqlite3_stmt *restt;
    int room_id;
    int rc = sqlite3_open("uchat.db", &db);

    if (rc != SQLITE_OK)
    {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return 0;
    }

    rc = sqlite3_prepare_v2(db, "SELECT ID FROM ROOMS WHERE NAME = ?", -1, &restt, 0);
    sqlite3_bind_text(restt, 1, name, -1, SQLITE_STATIC);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error int searching: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }
    rc = sqlite3_step(restt);
    room_id = sqlite3_column_int(restt, 0);

    sqlite3_finalize(restt);
    sqlite3_close(db);

    return room_id;
}

char *get_date_from_message(sqlite3 *db, char *text)
{
    sqlite3_stmt *restt;
    char *date = NULL;
    int rc = sqlite3_open("uchat.db", &db);

    if (rc != SQLITE_OK)
    {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return NULL;
    }

    rc = sqlite3_prepare_v2(db, "SELECT DATE FROM MESSAGES WHERE MESSAGE = ?", -1, &restt, 0);
    sqlite3_bind_text(restt, 1, text, -1, SQLITE_STATIC);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error int searching: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }
    rc = sqlite3_step(restt);
    date = mx_strdup((char *)sqlite3_column_text(restt, 0));

    sqlite3_finalize(restt);
    sqlite3_close(db);

    return date;
}

t_list *get_text_from_message(sqlite3 *db, int id)
{
    sqlite3_stmt *restt;
    t_list *text = NULL;
    int rc = sqlite3_open("uchat.db", &db);

    if (rc != SQLITE_OK)
    {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return NULL;
    }
    rc = sqlite3_prepare_v2(db, "SELECT MESSAGE FROM MESSAGES WHERE ID_ROOM = ?", -1, &restt, 0);
    sqlite3_bind_int(restt, 1, id);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error int searching: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }
    while (sqlite3_step(restt) == SQLITE_ROW)
    {
        char *text_msg = mx_strdup((char *)sqlite3_column_text(restt, 0));
        mx_push_back(&text, text_msg);
    }

    sqlite3_finalize(restt);
    sqlite3_close(db);

    return text;
}

t_list *get_idmessage_from_message(sqlite3 *db, int id)
{
    sqlite3_stmt *restt;
    t_list *text = NULL;
    int rc = sqlite3_open("uchat.db", &db);

    if (rc != SQLITE_OK)
    {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return NULL;
    }
    rc = sqlite3_prepare_v2(db, "SELECT ID FROM MESSAGES WHERE ID_ROOM = ?", -1, &restt, 0);
    sqlite3_bind_int(restt, 1, id);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error int searching: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }
    while (sqlite3_step(restt) == SQLITE_ROW)
    {
        char *text_msg = mx_strdup(mx_itoa(sqlite3_column_int(restt, 0)));
        mx_push_back(&text, text_msg);
    }

    sqlite3_finalize(restt);
    sqlite3_close(db);

    return text;
}
