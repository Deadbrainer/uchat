#include "../inc/client.h"

void apply_css(GtkWidget *widget, GtkStyleProvider *provider)
{
    gtk_style_context_add_provider(gtk_widget_get_style_context(widget), provider, G_MAXUINT);
    if (GTK_IS_CONTAINER(widget))
        gtk_container_forall(GTK_CONTAINER(widget), (GtkCallback)apply_css, provider);
}

void main_menu_test()
{
    main_menu();
    gtk_widget_destroy(window);
}

void login_menu()
{
    GtkWidget *box;
    GtkWidget *stack;
    GtkWidget *switcher;

    GtkWidget *log_username_entry, *log_password_entry;
    GtkWidget *log_ok_button;
    GtkWidget *log_hbox_username, *log_hbox_password, *log_hbox_error;
    GtkWidget *log_vbox;

    GtkWidget *reg_username_entry, *reg_password_entry, *reg_password_entry_repeat;
    GtkWidget *reg_ok_button;
    GtkWidget *reg_hbox_username, *reg_hbox_password, *reg_hbox_password_repeat, *reg_hbox_error;
    GtkWidget *reg_vbox;

    // Window creation
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "UCHAT");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 300);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    g_signal_connect(G_OBJECT(window), "delete-event", G_CALLBACK(closeApp), NULL);

    //text befory entry fields
    log_error_label = gtk_label_new(NULL);
    //get_error_label(&log_error_label, 1);
    gtk_label_set_width_chars(GTK_LABEL(log_error_label), 12);

    log_username_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text (GTK_ENTRY(log_username_entry), "Login");
    log_password_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text (GTK_ENTRY(log_password_entry), "Password");
    gtk_entry_set_visibility(GTK_ENTRY(log_password_entry), FALSE);

    log_ok_button = gtk_button_new_with_label("Sign in");

    g_signal_connect(G_OBJECT(log_ok_button), "clicked", G_CALLBACK(login_clicked_username), log_username_entry);
    g_signal_connect(G_OBJECT(log_ok_button), "clicked", G_CALLBACK(login_clicked_password), log_password_entry);

    log_hbox_username = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    log_hbox_password = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    log_hbox_error = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

    log_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    gtk_box_pack_start(GTK_BOX(log_hbox_username), log_username_entry, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(log_hbox_password), log_password_entry, TRUE, TRUE, 5);

    gtk_box_pack_start(GTK_BOX(log_hbox_error), log_error_label, TRUE, TRUE, 5);

    gtk_box_pack_start(GTK_BOX(log_vbox), log_hbox_username, FALSE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(log_vbox), log_hbox_password, FALSE, TRUE, 5);
    gtk_box_pack_end(GTK_BOX(log_vbox), log_ok_button, FALSE, TRUE, 0);
    gtk_box_pack_end(GTK_BOX(log_vbox), log_hbox_error, FALSE, TRUE, 5);

    // Registration
    reg_error_label = gtk_label_new(NULL);
    //get_error_label(&reg_error_label, 1);
    gtk_label_set_width_chars(GTK_LABEL(reg_error_label), 12);

    reg_username_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text (GTK_ENTRY(reg_username_entry), "Login");
    reg_password_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text (GTK_ENTRY(reg_password_entry), "Password");
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

    reg_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    gtk_box_pack_start(GTK_BOX(reg_hbox_username), reg_username_entry, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(reg_hbox_password), reg_password_entry, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(reg_hbox_password_repeat), reg_password_entry_repeat, TRUE, TRUE, 5);

    gtk_box_pack_start(GTK_BOX(reg_hbox_error), reg_error_label, TRUE, TRUE, 5);

    gtk_box_pack_start(GTK_BOX(reg_vbox), reg_hbox_username, FALSE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(reg_vbox), reg_hbox_password, FALSE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(reg_vbox), reg_hbox_password_repeat, FALSE, TRUE, 5);
    gtk_box_pack_end(GTK_BOX(reg_vbox), reg_ok_button, FALSE, TRUE, 0);
    gtk_box_pack_end(GTK_BOX(reg_vbox), reg_hbox_error, FALSE, TRUE, 5);

    // Switcher

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    stack = gtk_stack_new();
    switcher = gtk_stack_switcher_new();

    gtk_stack_add_titled(GTK_STACK(stack), log_vbox, "SIGN IN", "SIGN IN");
    gtk_stack_add_titled(GTK_STACK(stack), reg_vbox, "SIGN UP", "SIGN UP");

    gtk_widget_set_halign(switcher, GTK_ALIGN_CENTER);

    gtk_stack_switcher_set_stack(GTK_STACK_SWITCHER(switcher), GTK_STACK(stack));
    gtk_box_pack_start(GTK_BOX(box), switcher, FALSE, TRUE, 6);
    gtk_box_pack_start(GTK_BOX(box), stack, TRUE, TRUE, 6);

    // End
    gtk_container_add(GTK_CONTAINER(window), box);

    style_context = gtk_widget_get_style_context(window);
    provider = GTK_STYLE_PROVIDER(gtk_css_provider_new());

    gtk_css_provider_load_from_path(GTK_CSS_PROVIDER(provider), "client/src/style.css", NULL);
    apply_css(window, provider);

    gtk_widget_show_all(window);
}
