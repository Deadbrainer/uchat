#include "server.h"

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

    rv = sqlite3_prepare_v2(db, mx_strjoin("UPDATE into ROOMS values(?) WHERE ID = ", id), -1, &stmt, NULL);
    if (rv != SQLITE_OK)
    {
        mx_printstr("Prepare error in ROOMS\n");
    }

    usernames = mx_strjoin(usernames, "\v");
    usernames = mx_strjoin(usernames, username);

    sqlite3_bind_text(stmt, 2, usernames, -1, SQLITE_STATIC);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void add_idroom_into_user(sqlite3 *db, char *username, char *id)
{
    sqlite3_stmt *stmt;
    int rv = SQLITE_OK;
    char *id_room = get_idrooms_from_users(db, username);

    if (sqlite3_open("uchat.db", &db))
    {
        printf("Could not open the.db\n");
        exit(-1);
    }

    rv = sqlite3_prepare_v2(db, mx_strjoin("UPDATE into USERS values(?) WHERE NAME = ", username), -1, &stmt, NULL);
    if (rv != SQLITE_OK)
    {
        mx_printstr("Prepare error in ROOMS\n");
    }

    id_room = mx_strjoin(id_room, "\v");
    id_room = mx_strjoin(id_room, id);

    sqlite3_bind_text(stmt, 3, id_room, -1, SQLITE_STATIC);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
