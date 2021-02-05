#include "../inc/client.h"

void main_menu_test()
{
    main_menu();
    gtk_widget_destroy(welcome_window);
}

void login_menu(bool wrong_login)
{
    GtkWidget *box;
    GtkWidget *stack;
    GtkWidget *switcher;

    GtkWidget *log_username_label, *log_password_label;
    GtkWidget *log_username_entry, *log_password_entry;
    GtkWidget *log_ok_button;
    GtkWidget *log_hbox_username, *log_hbox_password, *log_hbox_error;
    GtkWidget *log_vbox;

    GtkWidget *reg_username_label, *reg_password_label, *reg_password_label_repeat;
    GtkWidget *reg_username_entry, *reg_password_entry, *reg_password_entry_repeat;
    GtkWidget *reg_ok_button;
    GtkWidget *reg_hbox_username, *reg_hbox_password, *reg_hbox_password_repeat, *reg_hbox_error;
    GtkWidget *reg_vbox;

    // Window creation
    welcome_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(welcome_window), "UCHAT");
    gtk_window_set_position(GTK_WINDOW(welcome_window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(welcome_window), 200, 200);
    gtk_container_set_border_width(GTK_CONTAINER(welcome_window), 10);

    g_signal_connect(G_OBJECT(welcome_window), "delete-event", G_CALLBACK(closeApp), NULL);

    //text befory entry fields
    log_username_label = gtk_label_new("Login: ");
    log_password_label = gtk_label_new("Password: ");
    log_error_label = gtk_label_new(NULL);
    get_error_label(&log_error_label, 1);
    //gtk_label_set_markup(GTK_LABEL(log_error_label), "<span foreground='#ff0000'>error</span>");
    gtk_label_set_width_chars(GTK_LABEL(log_username_label), 12);
    gtk_label_set_width_chars(GTK_LABEL(log_password_label), 12);
    gtk_label_set_width_chars(GTK_LABEL(log_error_label), 12);

    log_username_entry = gtk_entry_new();
    log_password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(log_password_entry), FALSE);

    log_ok_button = gtk_button_new_with_label("Sign in");

    g_signal_connect(G_OBJECT(log_ok_button), "clicked", G_CALLBACK(login_clicked_username), log_username_entry);
    g_signal_connect(G_OBJECT(log_ok_button), "clicked", G_CALLBACK(login_clicked_password), log_password_entry);

    log_hbox_username = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    log_hbox_password = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    log_hbox_error = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

    log_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    gtk_box_pack_start(GTK_BOX(log_hbox_username), log_username_label, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(log_hbox_username), log_username_entry, TRUE, FALSE, 5);

    gtk_box_pack_start(GTK_BOX(log_hbox_password), log_password_label, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(log_hbox_password), log_password_entry, TRUE, FALSE, 5);

    gtk_box_pack_start(GTK_BOX(log_hbox_error), log_error_label, TRUE, FALSE, 5);

    gtk_box_pack_start(GTK_BOX(log_vbox), log_hbox_username, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(log_vbox), log_hbox_password, FALSE, FALSE, 5);
    gtk_box_pack_end(GTK_BOX(log_vbox), log_ok_button, FALSE, FALSE, 5);
    gtk_box_pack_end(GTK_BOX(log_vbox), log_hbox_error, FALSE, FALSE, 5);

    // Registration

    reg_username_label = gtk_label_new("Login: ");
    reg_password_label = gtk_label_new("Password: ");
    reg_password_label_repeat = gtk_label_new("Repeat: ");
    reg_error_label = gtk_label_new(NULL);
    get_error_label(&reg_error_label, 1);
    gtk_label_set_width_chars(GTK_LABEL(reg_username_label), 12);
    gtk_label_set_width_chars(GTK_LABEL(reg_password_label), 12);
    gtk_label_set_width_chars(GTK_LABEL(reg_password_label_repeat), 12);
    gtk_label_set_width_chars(GTK_LABEL(reg_error_label), 12);

    reg_username_entry = gtk_entry_new();
    reg_password_entry = gtk_entry_new();
    reg_password_entry_repeat = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(reg_password_entry), FALSE);
    gtk_entry_set_visibility(GTK_ENTRY(reg_password_entry_repeat), FALSE);

    reg_ok_button = gtk_button_new_with_label("Sign up");
    g_signal_connect(G_OBJECT(reg_ok_button), "clicked", G_CALLBACK(reg_clicked_username), reg_username_entry);
    g_signal_connect(G_OBJECT(reg_ok_button), "clicked", G_CALLBACK(reg_clicked_password), reg_password_entry);
    g_signal_connect(G_OBJECT(reg_ok_button), "clicked", G_CALLBACK(reg_clicked_password_repeat), reg_password_entry_repeat);

    reg_hbox_username = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    reg_hbox_password = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    reg_hbox_password_repeat = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    reg_hbox_error = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

    reg_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    gtk_box_pack_start(GTK_BOX(reg_hbox_username), reg_username_label, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(reg_hbox_username), reg_username_entry, TRUE, FALSE, 5);

    gtk_box_pack_start(GTK_BOX(reg_hbox_password), reg_password_label, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(reg_hbox_password), reg_password_entry, TRUE, FALSE, 5);

    gtk_box_pack_start(GTK_BOX(reg_hbox_password_repeat), reg_password_label_repeat, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(reg_hbox_password_repeat), reg_password_entry_repeat, TRUE, FALSE, 5);

    gtk_box_pack_start(GTK_BOX(reg_hbox_error), reg_error_label, TRUE, FALSE, 5);

    gtk_box_pack_start(GTK_BOX(reg_vbox), reg_hbox_username, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(reg_vbox), reg_hbox_password, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(reg_vbox), reg_hbox_password_repeat, FALSE, FALSE, 5);
    gtk_box_pack_end(GTK_BOX(reg_vbox), reg_ok_button, FALSE, FALSE, 5);
    gtk_box_pack_end(GTK_BOX(reg_vbox), reg_hbox_error, FALSE, FALSE, 5);

    // Switcher

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    stack = gtk_stack_new();
    switcher = gtk_stack_switcher_new();

    gtk_stack_add_titled(GTK_STACK(stack), log_vbox, "Sign in", "Sign in");
    gtk_stack_add_titled(GTK_STACK(stack), reg_vbox, "Sign up", "Sign up");

    gtk_widget_set_halign(switcher, GTK_ALIGN_CENTER);

    gtk_stack_switcher_set_stack(GTK_STACK_SWITCHER(switcher), GTK_STACK(stack));
    gtk_box_pack_start(GTK_BOX(box), switcher, FALSE, FALSE, 6);
    gtk_box_pack_start(GTK_BOX(box), stack, TRUE, TRUE, 6);

    // End
    gtk_container_add(GTK_CONTAINER(welcome_window), box);

    g_signal_connect(G_OBJECT(welcome_window), "destroy", gtk_main_quit, NULL);

    gtk_widget_show_all(welcome_window);
}
