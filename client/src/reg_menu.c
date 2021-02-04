#include "../inc/client.h"

void reg_menu(GtkWidget *button, gpointer data)
{
    GtkWidget *username_label, *password_label;
    GtkWidget *username_entry, *password_entry;
    GtkWidget *hbox1, *hbox2, *hbox3, *vbox;
    GtkWidget *reg_button, *back_button;

    // window creation
    reg_window = gtk_window_new(GTK_WINDOW_POPUP);
    new_window(reg_window, -1, -1, FALSE, 10, "Register");

    // switch back to login menu
    //g_signal_connect(G_OBJECT(reg_window), "destroy", G_CALLBACK(window_switch), reg_window);

    //text befory entry, fields and button
    username_label = gtk_label_new("Login: ");
    gtk_label_set_width_chars(GTK_LABEL(username_label), 10);
    password_label = gtk_label_new("Password: ");
    gtk_label_set_width_chars(GTK_LABEL(password_label), 10);

    username_entry = gtk_entry_new();
    password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE); //* makes password hidden

    reg_button = gtk_button_new_with_label("Register new account");
    back_button = gtk_button_new_with_label("<-");
    g_signal_connect(G_OBJECT(back_button), "clicked", G_CALLBACK(window_switch), reg_window);

    //Packing buttons and fields
    hbox1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    hbox2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    hbox3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    gtk_box_pack_start(GTK_BOX(hbox1), username_label, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox1), username_entry, TRUE, FALSE, 5);

    gtk_box_pack_start(GTK_BOX(hbox2), password_label, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox2), password_entry, TRUE, FALSE, 5);

    gtk_box_pack_start(GTK_BOX(hbox3), back_button, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox3), reg_button, TRUE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(vbox), hbox1, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), hbox2, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), hbox3, FALSE, FALSE, 5);

    //required to show everything
    gtk_container_add(GTK_CONTAINER(reg_window), vbox);
    gtk_widget_show_all(reg_window);
}
