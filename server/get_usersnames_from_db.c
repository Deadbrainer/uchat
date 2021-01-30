#include "server.h"

t_list* get_names(sqlite3* db) {
    sqlite3_stmt *res;

    int rc = sqlite3_open("uchat.db", &db);

    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return NULL;
    }

    char* sql = "SELECT name FROM USERS";
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

    t_list* ss = NULL;

    while (sqlite3_step(res) == SQLITE_ROW) {

        char* a = (char*)sqlite3_column_text(res, 0);

        mx_push_back(&ss, strdup(a));
    }

    sqlite3_finalize(res);
    sqlite3_close(db);

    return ss;
}
