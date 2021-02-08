#include "server.h"

void insert_into_db_users(sqlite3 *db, char *name, char *password)
{
    //static int count_id = 1;
    sqlite3_stmt *stmt;
    int rv = SQLITE_OK;

    if (sqlite3_open("uchat.db", &db))
    {
        printf("Could not open the.db\n");
        exit(-1);
    }

    rv = sqlite3_prepare_v2(db, "insert into USERS values(?, ?);", -1, &stmt, NULL);
    if (rv != SQLITE_OK)
    {
        mx_printstr("Prepare error in USERS\n");
    }

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    // sqlite3_bind_text(stmt, 2, user->login, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);
    // sqlite3_bind_text(stmt, 4, user->token, -1, SQLITE_STATIC);
    // sqlite3_bind_int64(stmt, 5, user->date);
    // sqlite3_bind_text(stmt, 6, user->desc, -1, SQLITE_STATIC);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    //count_id++;
}
