#include "../inc/uchat.h"

void button_clicked(GtkWidget *button, gpointer data) // TODO: THIS
{
    const char *login_text = gtk_entry_get_text(GTK_ENTRY((GtkWidget *)data));
    const char *password_text = gtk_entry_get_text(GTK_ENTRY((GtkWidget *)data));
    /*if (strcmp(password_text, password) == 0)
        printf("Access granted!\n");
    else
        printf("Access denied!\n");*/
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);
    login_menu();
    gtk_main();
    return 0;
}
