#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "../../libmx/inc/libmx.h"
#include <sqlite3.h>
#include <stdbool.h>

// Database Functions
// Create Tables
void create_table(sqlite3 *db);
void create_table_users(sqlite3 *db, char *zErrMsg, int rc, char *sql);
void create_table_messages(sqlite3 *db, char *zErrMsg, int rc, char *sql);
void create_table_rooms(sqlite3 *db, char *zErrMsg, int rc, char *sql);

// Insert into db
void insert_into_db_users(sqlite3 *db, char *name, char *password, int sock_id);
void insert_into_db_message(sqlite3 *db, char *room_id, char *name, char *msg);
void insert_into_db_room(sqlite3 *db, char *room_name, char *username);
void add_idroom_into_user(sqlite3 *db, char *username, int id);
void add_user_into_room(sqlite3 *db, char *username, char *id);
void add_sockid_into_user(sqlite3 *db, char *username, int sockid);
void add_newtext_into_mesage(sqlite3 *db, char *new_text, char *id);

// Get from db
t_list *get_usernames_from_db(sqlite3 *db);
char *get_password_from_db(sqlite3 *db, char *name);
char *get_idrooms_from_users(sqlite3 *db, char *username);
char *get_roomnames_from_rooms(sqlite3 *db, char *id);
char *get_usernames_from_rooms(sqlite3 *db, char *id);
char *get_roomid_from_users(sqlite3 *db, char *name);
int get_sockid_from_db(sqlite3 *db, char *name);
char *get_roomnames_from_rooms_by_roomname(sqlite3 *db, char *roomname);
int get_roomid_from_room_with_roomname(sqlite3 *db, char *name);
char *get_date_from_message(sqlite3 *db, char *name);
t_list *get_text_from_message(sqlite3 *db, int id);
t_list *get_idmessage_from_message(sqlite3 *db, int id);

//Getters
void getDataBase(sqlite3 *x, int check);
void get_password(char **password, int check);
void get_mutex(pthread_mutex_t *x, int check);
void get_list(t_list **x, int check);
char *get_date();
