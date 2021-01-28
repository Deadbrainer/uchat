#include "server.h"

void insert_into_db(sqlite3 *db, char *name, char *password)
{
    static int count_id = 1;
    sqlite3_stmt *stmt;
    int rv = SQLITE_OK;

    if (sqlite3_open("uchat.db", &db))
    {
        printf("Could not open the.db\n");
        exit(-1);
    }

    // time_t rawtime;
    // struct tm* timeinfo;

    // time(&rawtime);
    // timeinfo = localtime(&rawtime);

    rv = sqlite3_prepare_v2(db, "insert into USERS values(?, ?, ?);", -1, &stmt, NULL);
    if (rv != SQLITE_OK)
    {
        mx_printstr("Pizda\n");
    }

    mx_error_sqlite(rv, "insert USERS into db");

    sqlite3_bind_int(stmt, 1, count_id);
    sqlite3_bind_text(stmt, 2, name, -1, SQLITE_STATIC);
    // sqlite3_bind_text(stmt, 2, user->login, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, password, -1, SQLITE_STATIC);
    // sqlite3_bind_text(stmt, 4, user->token, -1, SQLITE_STATIC);
    // sqlite3_bind_int64(stmt, 5, user->date);
    // sqlite3_bind_text(stmt, 6, user->desc, -1, SQLITE_STATIC);

    // mx_error_sqlite(sqlite3_step(stmt), "insert user into db");
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    count_id++;
    // get_id_user(db, user);
}
