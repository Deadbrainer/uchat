#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "../libmx/inc/libmx.h"
#include <sqlite3.h>
#include <stdbool.h>

// Database Functions
void create_table(sqlite3 *db);
void create_table_users(sqlite3 *db, char *zErrMsg, int rc, char *sql);
void create_table_messages(sqlite3 *db, char *zErrMsg, int rc, char *sql);
int callback(void *NotUsed, int argc, char **argv, char **azColName);
void get_from_db_users(sqlite3 *db);
void insert_into_db_users(sqlite3 *db, char *name, char *password);
t_list *get_usernames_from_db(sqlite3 *db);
void insert_into_db_message(sqlite3 *db, char *name, char *msg);
void get_from_db_messages(sqlite3 *db);
char *get_password_from_db(sqlite3 *db, char *name);
