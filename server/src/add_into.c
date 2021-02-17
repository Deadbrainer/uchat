#include "../inc/server.h"

void add_user_into_room(sqlite3 *db, char *username, char *id)
{
    sqlite3_stmt *stmt;
    int rv = SQLITE_OK;
    char *usernames = get_usernames_from_rooms(db, id);

    if (sqlite3_open("uchat.db", &db))
    {
        printf("Could not open the.db\n");
        exit(-1);
    }

    rv = sqlite3_prepare_v2(db, "UPDATE ROOMS SET USERNAMES = ? WHERE ID = ?", -1, &stmt, NULL);
    usernames = mx_strjoin(usernames, "\v");
    usernames = mx_strjoin(usernames, username);

    sqlite3_bind_text(stmt, 1, usernames, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, id, -1, SQLITE_STATIC);

    if (rv != SQLITE_OK)
    {
        fprintf(stderr, "Error int searching: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
    }

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void add_idroom_into_user(sqlite3 *db, char *username, int id)
{
    sqlite3_stmt *stmt;
    int rv = SQLITE_OK;
    char *id_room = get_idrooms_from_users(db, username);

    if (sqlite3_open("uchat.db", &db))
    {
        printf("Could not open the.db\n");
        exit(-1);
    }

    rv = sqlite3_prepare_v2(db, "UPDATE USERS SET ID_ROOMS = ? WHERE NAME = ?", -1, &stmt, NULL);

    id_room = mx_strjoin(id_room, "\v");
    id_room = mx_strjoin(id_room, mx_itoa(id));

    sqlite3_bind_text(stmt, 1, id_room, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, username, -1, SQLITE_STATIC);

    if (rv != SQLITE_OK)
    {
        fprintf(stderr, "Error int searching: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
    }

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
// UPDATE sockid
void add_sockid_into_user(sqlite3 *db, char *username, int sockid)
{
    sqlite3_stmt *stmt;
    int rv = SQLITE_OK;

    if (sqlite3_open("uchat.db", &db))
    {
        printf("Could not open the.db\n");
        exit(-1);
    }

    rv = sqlite3_prepare_v2(db, "UPDATE USERS SET SOCK_ID = ? WHERE NAME = ?", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, sockid);
    sqlite3_bind_text(stmt, 2, username, -1, SQLITE_STATIC);

    if (rv != SQLITE_OK)
    {
        mx_printstr("Prepare error in ROOMS\n");
    }

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void add_newtext_into_mesage(sqlite3 *db, char *newtext, char *id)
{
    sqlite3_stmt *stmt;
    int rv = SQLITE_OK;

    if (sqlite3_open("uchat.db", &db))
    {
        printf("Could not open the.db\n");
        exit(-1);
    }

    rv = sqlite3_prepare_v2(db, "UPDATE MESSAGES SET TEXT = ? WHERE ID = ?", -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, newtext, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, mx_atoi(id));

    if (rv != SQLITE_OK)
    {
        mx_printstr("Prepare error in ROOMS\n");
    }

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
