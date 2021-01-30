#pragma once

#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

GtkWidget *log_window;
GtkWidget *reg_window;

/*
g_signal_connect(G_OBJECT(log_window), "delete-event", G_CALLBACK(closeApp), NULL);

G_OBJECT(log_window): log_window - в каком окне мы слушаем, 
G_CALLBACK(closeApp): closeApp - какую функцию мы вызываем. Передать окно? Не, нихуя. 
Там после него идет NULL только там можно передать gpointer data

текстовая херня - "clicked"/"Delete-event"/"destroy"
https://developer.gnome.org/gtk3/stable/GtkWidget.html -- Крутим к оглавлению сигналов
Там есть все сигналы
*/

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
void button_clicked(GtkWidget *button, gpointer data);

//Opens new window for registering
void reg_menu();

//Closes whole application
void closeApp(GtkWidget *window, gpointer data);
