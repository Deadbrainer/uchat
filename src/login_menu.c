#include "../inc/uchat.h"

void main_menu_test()
{
    main_menu();
    gtk_widget_destroy(log_window);
}

//// void button_clicked(GtkWidget *button, gpointer data) // TODO: THIS
//// {
////     const char *login_text;
////     const char *password_text;
////     int sock = 0;
////     get_sockid(&sock, 0);
////     fprintf(stderr, "%d\n", sock);
////     login_text = gtk_entry_get_text(GTK_ENTRY((GtkWidget *)data));
////     password_text = gtk_entry_get_text(GTK_ENTRY((GtkWidget *)data));
////     char *to_send = mx_strjoin(login_text, " ");
////     to_send = mx_strjoin(to_send, password_text);
////     if (send(sock, to_send, mx_strlen(to_send), 0) < 0)
////     {
////         fprintf(stderr, "sending failure\n");
////     }
////     int len = 0;
////     char* rec = mx_strnew(8);
////     len = recv(sock, rec, 32, 0);
////     printf("GOT: %s\n", rec);
////     if (mx_strcmp(rec, "N") == 0) {
////         printf("NEPRAVULNO\n");
////     } else if (mx_strcmp(rec, "Y") == 0){
////         printf("PRAVULNO\n");
////     } else {
////         printf("NIHUYA NE PRISHLO\n");
////     }
////     //send(sock, to_send, mx_strlen(to_send), 0);
////     /*if (strcmp(password_text, password) == 0)
////         printf("Access granted!\n");
////     else
////         printf("Access denied!\n");*/
//// }

void login_menu()
{
    GtkWidget *username_label, *password_label;
    GtkWidget *username_entry, *password_entry;
    GtkWidget *enter_button, *button_check;
    GtkWidget *hbox1, *hbox2, *vbox;
    GtkWidget *reg_button;
    // Window creation
    log_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    new_window(log_window, -1, -1, FALSE, 2, "Uchat Log In");

    // application destruction
    g_signal_connect(G_OBJECT(log_window), "delete-event", G_CALLBACK(closeApp), NULL); //пояснение сигналов в хедере


    //text befory entry fields
    username_label = gtk_label_new("Login: ");
    gtk_label_set_width_chars(GTK_LABEL(username_label), 10);
    password_label = gtk_label_new("Password: ");
    gtk_label_set_width_chars(GTK_LABEL(password_label), 10);

    //entry fields
    username_entry = gtk_entry_new();
    password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE); //* makes password hidden

    enter_button = gtk_button_new_with_label("Login");
    //g_signal_connect(G_OBJECT(enter_button), "clicked", G_CALLBACK(button_clicked), username_entry);
    g_signal_connect(G_OBJECT(enter_button), "clicked", G_CALLBACK(main_menu_test), NULL);

    /* Рома: мы не настолько продвинутые (51 line)
    button_check = gtk_check_button_new_with_label("Remember password?"); // TODO: This, without listener BTW */

    //button listener
    reg_button = gtk_button_new_with_label("Register");
    g_signal_connect(G_OBJECT(reg_button), "clicked", G_CALLBACK(window_switch), reg_window); //Opens reg window

    //Packing buttons and fields
    hbox1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    hbox2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    gtk_box_pack_start(GTK_BOX(hbox1), username_label, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox1), username_entry, TRUE, FALSE, 5);

    gtk_box_pack_start(GTK_BOX(hbox2), password_label, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox2), password_entry, TRUE, FALSE, 5);

    gtk_box_pack_start(GTK_BOX(vbox), hbox1, FALSE, FALSE, 18);
    gtk_box_pack_start(GTK_BOX(vbox), hbox2, FALSE, FALSE, 0);
    //gtk_box_pack_start(GTK_BOX(vbox), button_check, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), enter_button, FALSE, FALSE, 9);
    gtk_box_pack_start(GTK_BOX(vbox), reg_button, FALSE, FALSE, 5);

    //required to show everything
    gtk_container_add(GTK_CONTAINER(log_window), vbox);
    gtk_widget_show_all(log_window);
}
