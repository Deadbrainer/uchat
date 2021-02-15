#pragma once

#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "../../libmx/inc/libmx.h"
#include <signal.h>

GtkWidget *log_error_label;
GtkWidget *reg_error_label;
GtkWidget *roomname_error_label;
GtkWidget *add_user_error_label;
GtkWidget *window;
GtkWidget *main_window;

GtkStyleContext *style_context;
GtkStyleProvider *provider;

void apply_css(GtkWidget *widget, GtkStyleProvider *provider);
/** Creates new top-level window 
 * @param win which window
 * @param x width in pixels, or -1 to unset the default width
 * @param z height in pixels, or -1 to unset the default height
 * @param resize  Sets whether the user can resize a window.
 * @param border  The border width of a container is the amount of space to leave around the outside of the container.
 * @param title  Sets the title of the GtkWindow
**/
void new_window(GtkWidget *win, int x, int z, bool resize, int border, char title[]);

/** Closes window and opens new. Data rule: put from which window it's calling
 * @param window window to close
 * @param data what function to execute
*/
void window_switch(GtkWidget *window, gpointer data);

//Opens login menu
void login_menu();

//Макс блять сделай с этим, что-то. Оно ща в uchat.c (Я на 90% уверен, что он нихуя не сделает)
void login_clicked_username(GtkWidget *button, gpointer data);

//Opens new window for registering
void reg_menu();

//Closes whole application
void closeApp(GtkWidget *window, gpointer data);

void get_sockid(int *sockid, int check);

void init_authorization(void);
void init_chat(void);
void popup_add_chat(void);
void popup_add_user(gpointer data);

//void add_chat(const gchar *title);
GtkWidget *add_chat(GtkWidget **stack, char *text);

void main_menu();

//Getters
void get_if_login_ok(bool *flag, int check);
void get_login(char **login, int check);
void get_array(char **array, int check);
void get_password(char **password, int check);
void get_text_entry(GtkWidget **textEntry, bool check);
void get_buffer(GtkTextBuffer **buffer, bool check);
void get_iter(GtkTextIter *iter, bool check);
void get_list(t_list **x, int check);
char *get_date();
void get_struct_socaddr(struct sockaddr_in **SocketIp, int check);

//Registration Handler
void reg_clicked_username(GtkWidget *button, gpointer data);
void reg_clicked_password(GtkWidget *button, gpointer data);
void reg_clicked_password_repeat(GtkWidget *button, gpointer data);

//Signin Handler
void login_clicked_username(GtkWidget *button, gpointer data);
void login_clicked_password(GtkWidget *button, gpointer data);

void main_menu_test();

void add_rooms();

void *recvmg_new(void *my_sock);

// chat
GtkWidget *chat_stack;
const gchar *current_roomname;

//*useless shit >
GtkWidget *chat_text_area;
GtkWidget *chat_text_scrolled_window;
GtkWidget *chat_text_vbox;
GtkWidget *chat_text_send_button, *chat_text_add_button;
GtkWidget *chat_text_hbox;
GtkWidget *chat_text_entry;

// popup add_chat
GtkWidget *add_chat_dialog;
GtkWidget *add_chat_label;
GtkWidget *add_chat_entry;
GtkWidget *add_chat_content_area;

// popup add_user
GtkWidget *add_user_dialog;
GtkWidget *add_user_label;
GtkWidget *add_user_entry;
GtkWidget *add_user_content_area;

//GtkWidget *stack;
GtkWidget *textAction(GtkWidget **stack, char *text);
// void handle(int signum);
gboolean handle(gpointer user_data);

pid_t pid;
