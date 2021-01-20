#include <stdlib.h>
#include <sqlite3.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName);
void create_table(sqlite3 *db);
void insert_into_db(sqlite3 *db);