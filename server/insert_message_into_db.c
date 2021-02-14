#include "server.h"
#include <time.h>

void insert_into_db_message(sqlite3 *db, char *room_id, char *name, char *msg)
{
    sqlite3_stmt *stmt;
    int rv = SQLITE_OK;

    if (sqlite3_open("uchat.db", &db))
    {
        printf("Could not open the.db\n");
        exit(-1);
    }

    rv = sqlite3_prepare_v2(db, "insert into MESSAGES values(?, ?, ?, ?);", -1, &stmt, NULL);
    if (rv != SQLITE_OK)
    {
        mx_printstr("Prepare error in MESSAGES\n");
    }

    sqlite3_bind_text(stmt, 1, room_id, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, get_date(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, msg, -1, SQLITE_STATIC);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
