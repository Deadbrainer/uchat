#include "../inc/client.h"

void auto_scroll()
{
    GtkAdjustment *vadjustment;
    GtkWidget *vbox = gtk_stack_get_visible_child(GTK_STACK(chat_stack));
    GList *list1 = gtk_container_get_children(GTK_CONTAINER(vbox));
    GtkContainer *scroll = GTK_CONTAINER(list1->next->data);
    vadjustment = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(scroll));

    gtk_adjustment_set_value(vadjustment, (gtk_adjustment_get_upper(vadjustment) - gtk_adjustment_get_page_size(vadjustment)));
}
