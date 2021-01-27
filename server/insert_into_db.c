#include "server.h"

void insert_into_db(sqlite3 *db, char* name, char* password)
{

    sqlite3_stmt *stmt;
    int rv = SQLITE_OK;

    // time_t rawtime;
    // struct tm* timeinfo;

    // time(&rawtime);
    // timeinfo = localtime(&rawtime);


    rv = sqlite3_prepare_v2(db, "insert into USERS(NAME, PASSWORD)values(?1, ?2);", -1, &stmt, NULL);

    mx_error_sqlite(rv, "insert USERS into db");

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    // sqlite3_bind_text(stmt, 2, user->login, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, password, -1, SQLITE_STATIC);
    // sqlite3_bind_text(stmt, 4, user->token, -1, SQLITE_STATIC);
    // sqlite3_bind_int64(stmt, 5, user->date);
    // sqlite3_bind_text(stmt, 6, user->desc, -1, SQLITE_STATIC);

    mx_error_sqlite(sqlite3_step(stmt), "insert user into db");
    sqlite3_finalize(stmt);

    // get_id_user(db, user);
}

