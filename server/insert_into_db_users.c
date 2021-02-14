#include "server.h"

void insert_into_db_users(sqlite3 *db, char *name, char *password, int sock_id)
{
    sqlite3_stmt *stmt;
    int rv = SQLITE_OK;

    if (sqlite3_open("uchat.db", &db))
    {
        printf("Could not open the.db\n");
        exit(-1);
    }

    rv = sqlite3_prepare_v2(db, "insert into USERS values(?, ?, ?, ?);", -1, &stmt, NULL);
    if (rv != SQLITE_OK)
    {
        mx_printstr("Prepare error in USERS\n");
    }

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, sock_id);
    sqlite3_bind_text(stmt, 4, "0", -1, SQLITE_STATIC);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
