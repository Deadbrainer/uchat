#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "../libmx/inc/libmx.h"
#include <sqlite3.h>
#include <stdbool.h>

void create_table(sqlite3 *db);

int callback(void *NotUsed, int argc, char **argv, char **azColName);

bool mx_error_sqlite(int rv, char *error);

void get_from_db_users(sqlite3 *db);

void insert_into_db(sqlite3 *db, char* name, char* password);


