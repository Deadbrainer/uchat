#include "server.h"

// Get ID rooms
char **get_idrooms_from_users(sqlite3 *db)
{
    char **res;
    char *a;

    int rc = sqlite3_open("uchat.db", &db);

    if (rc != SQLITE_OK)
    {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return NULL;
    }

    char *sql = "SELECT ID_ROOMS FROM USERS";
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

    t_list *ss = NULL;

    while (sqlite3_step(res) == SQLITE_ROW)
    {

        a = (char *)sqlite3_column_text(res, 0);
    }

    res = mx_strsplit(a, ' ');

    sqlite3_finalize(res);
    sqlite3_close(db);

    return res;
}

// Get Roomnames
char *get_roomnames_from_rooms(sqlite3 *db, char *id)
{
    sqlite3_stmt *res;
    char *room_name;

    int rc = sqlite3_open("uchat.db", &db);

    if (rc != SQLITE_OK)
    {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return NULL;
    }

    char *sql = "SELECT NAME FROM ROOMS WHERE ID = ";
    sql = mx_strjoin(sql, id);
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

    while (sqlite3_step(res) == SQLITE_ROW)
    {
        room_name = (char *)sqlite3_column_text(res, 0);
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

    char *sql = "SELECT USERNAMES FROM ROOMS WHERE ID = ";
    sql = mx_strjoin(sql, id);
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

    while (sqlite3_step(res) == SQLITE_ROW)
    {
        user_name = (char *)sqlite3_column_text(res, 0);
    }

    sqlite3_finalize(res);
    sqlite3_close(db);

    return user_name;
}
// Get room ids from users
char *get_roomid_from_users(sqlite3 *db, char *name)
{
    sqlite3_stmt *res;
    char *roomids;

    int rc = sqlite3_open("uchat.db", &db);

    if (rc != SQLITE_OK)
    {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return NULL;
    }

    char *sql = "SELECT ID_ROOMS FROM USERS WHERE NAME = ";
    sql = mx_strjoin(sql, name);
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

    while (sqlite3_step(res) == SQLITE_ROW)
    {
        roomids = (char *)sqlite3_column_text(res, 0);
    }

    sqlite3_finalize(res);
    sqlite3_close(db);

    return roomids;
}

// Get password
char *get_password_from_db(sqlite3 *db, char *name)
{
    sqlite3_stmt *res;
    char *password;

    int rc = sqlite3_open("uchat.db", &db);

    if (rc != SQLITE_OK)
    {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return NULL;
    }

    char *sql = "SELECT password FROM USERS WHERE NAME = ";
    sql = mx_strjoin(sql, name);
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

    while (sqlite3_step(res) == SQLITE_ROW)
    {
        password = (char *)sqlite3_column_text(res, 0);
    }

    sqlite3_finalize(res);
    sqlite3_close(db);

    return password;
}

// Get usernames
t_list *get_usernames_from_db(sqlite3 *db)
{
    sqlite3_stmt *res;

    int rc = sqlite3_open("uchat.db", &db);

    if (rc != SQLITE_OK)
    {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return NULL;
    }

    char *sql = "SELECT name FROM USERS";
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

    t_list *ss = NULL;

    while (sqlite3_step(res) == SQLITE_ROW)
    {

        char *a = (char *)sqlite3_column_text(res, 0);

        mx_push_back(&ss, strdup(a));
    }

    sqlite3_finalize(res);
    sqlite3_close(db);

    return ss;
}
