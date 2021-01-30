#include "server.h"

t_list *get_usernames_from_db(sqlite3 *db)
{
    t_list *names = NULL;
    sqlite3_stmt *stmt;

    int rc = sqlite3_open("uchat.db", &db);

    if (rc != SQLITE_OK)
    {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
    }
    rc = sqlite3_prepare_v2(db, "SELECT NAME FROM USERS", -1, &stmt, 0);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        // const unsigned char *a = sqlite3_column_text(stmt, 0);
        // printf("%s\n", a);
        //mx_push_back(&names, (void *)sqlite3_column_text(stmt, 0));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return names;
}