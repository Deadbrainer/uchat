#include "server.h"

void get_rooms_from_db(sqlite3 *db)
{
    char *zErrMsg = 0;
    int rc;
    char *sql;
    const char *data = "Callback function called";

    /* Open database */
    rc = sqlite3_open("uchat.db", &db);

    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    }
    else
    {
        fprintf(stderr, "Opened database successfully\n");
    }

    /* Create SQL statement */
    sql = "SELECT * from ROOMS";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, (void *)data, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Operation done successfully\n");
    }
    sqlite3_close(db);
}
