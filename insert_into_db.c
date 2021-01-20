#include "server.h"

void insert_into_db(sqlite3 *db)
{
    char *zErrMsg = 0;
    int rc;
    char *sql;

    /* Open database */
    rc = sqlite3_open("test.db", &db);

    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    }
    else
    {
        fprintf(stderr, "Opened database successfully\n");
    }

    /* Create SQL statement */
    sql = "INSERT INTO USERS (ID,NAME,PASSWORD) "
          "VALUES (1, 'Paul', 3232); "
          "INSERT INTO USERS (ID,NAME,PASSWORD) "
          "VALUES (2, 'Allen', 2525); "
          "INSERT INTO USERS (ID,NAME,PASSWORD)"
          "VALUES (3, 'Teddy', 2323);"
          "INSERT INTO USERS (ID,NAME,PASSWORD)"
          "VALUES (4, 'Mark', 2525);";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Records created successfully\n");
    }
    sqlite3_close(db);
}