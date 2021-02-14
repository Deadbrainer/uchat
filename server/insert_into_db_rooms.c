#include "server.h"

void insert_into_db_room(sqlite3 *db, char *room_name, char *username)
{
    sqlite3_stmt *stmt;
    int rv = SQLITE_OK;

    if (sqlite3_open("uchat.db", &db))
    {
        printf("Could not open the.db\n");
        exit(-1);
    }

    rv = sqlite3_prepare_v2(db, "insert into ROOMS values(?, ?, ?);", -1, &stmt, NULL);
    if (rv != SQLITE_OK)
    {
        mx_printstr("Prepare error in ROOMS\n");
    }
    sqlite3_bind_text(stmt, 2, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, room_name, -1, SQLITE_STATIC);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
