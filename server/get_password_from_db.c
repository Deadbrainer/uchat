#include "server.h"

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
        printf("%s\n", password);
    }

    sqlite3_finalize(res);
    sqlite3_close(db);

    return password;
}