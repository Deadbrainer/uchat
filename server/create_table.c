#include "server.h"

void create_table(sqlite3 *db)
{
    char *zErrMsg = 0;
    int rc;
    char *sql = NULL;

    /* Open database */
    rc = sqlite3_open("uchat.db", &db);
    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    }
    else
    {
        fprintf(stdout, "Opened database successfully\n");
    }

    create_table_users(db, zErrMsg, rc, sql);
    create_table_messages(db, zErrMsg, rc, sql);

    sqlite3_close(db);
}

void create_table_users(sqlite3 *db, char *zErrMsg, int rc, char *sql)
{

    /* Create SQL statement */
    sql = "CREATE TABLE IF NOT EXISTS USERS("
          "ID             INT      NOT NULL,"
          "NAME           TEXT    NOT NULL,"
          "PASSWORD       TEXT    NOT NULL);";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Table USER created successfully\n");
    }
}

void create_table_messages(sqlite3 *db, char *zErrMsg, int rc, char *sql)
{
    /* Create SQL statement */
    sql = "CREATE TABLE IF NOT EXISTS MESSAGES("
          //"ID            INT      NOT NULL,"
          "USERNAME      TEXT    NOT NULL,"
          "MESSAGE       TEXT    NOT NULL);";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Table MESSAGES created successfully\n");
    }
}
