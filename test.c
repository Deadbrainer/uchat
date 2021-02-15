#include <stdio.h>
#include <gtk/gtk.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	GtkWidget *window = NULL;
	GtkWidget *sidebar;
	GtkWidget *stack;
	GtkWidget *outbutton;
	GtkWidget *hbox;
	GtkWidget *vbox;
	GtkWidget *message_label;
	GtkWidget *header;
	GtkWidget *widget;
	GtkWidget *content_area;
	GtkWidget *toolbar;
	GtkWidget *bar;
	GtkWidget *button;
	GtkWidget *button2;
	const char *pages[] = {
		"Welcome to Gtk+3.0",
		"page2",
		"page3",
		NULL
	};
	const char *c = NULL;
 
	gtk_init(&argc,&argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
	gtk_widget_set_size_request(window, 600, 400);
 
	header = gtk_header_bar_new();
	gtk_window_set_titlebar(GTK_WINDOW(window), header);
	gtk_window_set_title(GTK_WINDOW(window), "Stack Sidebar");
	gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(header), TRUE);
	gtk_header_bar_set_decoration_layout(GTK_HEADER_BAR(header), "menu:minimize,close");
 
    toolbar = gtk_toolbar_new();
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
 
    bar = gtk_info_bar_new();
    message_label = gtk_label_new ("message");
    content_area = gtk_info_bar_get_content_area ((GtkInfoBar *)bar);
	gtk_container_add (GTK_CONTAINER (content_area), message_label);
	gtk_info_bar_add_button ((GtkInfoBar *)bar, ("_OKAA"), GTK_RESPONSE_OK);
	gtk_info_bar_set_show_close_button ((GtkInfoBar *)bar, TRUE);
	g_signal_connect ((GtkInfoBar *)bar, "response", G_CALLBACK (gtk_widget_hide), NULL);
	gtk_info_bar_set_message_type ((GtkInfoBar *)bar, GTK_MESSAGE_QUESTION);
 
    button = GTK_WIDGET(gtk_tool_button_new(NULL, NULL));
    gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(button), "view-refresh-symbolic");
	gtk_header_bar_pack_start(GTK_HEADER_BAR(header), button);
 
    button = gtk_button_new_with_label("button");
	gtk_header_bar_pack_end(GTK_HEADER_BAR(header), button);
 
	button = GTK_WIDGET(gtk_tool_button_new(NULL, NULL));
    gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(button), "go-home");
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), (GtkToolItem *)button, -1);
    
    button = GTK_WIDGET(gtk_tool_button_new(NULL, NULL));
    gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(button), "view-fullscreen");
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), (GtkToolItem *)button, -1);
 
	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	sidebar = gtk_stack_sidebar_new();
	gtk_box_pack_start(GTK_BOX(hbox), sidebar, FALSE, FALSE, 0);
 
	stack = gtk_stack_new();
	Gtk_stack_set_transition_type(GTK_STACK(stack), GTK_STACK_TRANSITION_TYPE_SLIDE_UP);//The effect of switching is upward
	gtk_stack_sidebar_set_stack(GTK_STACK_SIDEBAR(sidebar), GTK_STACK(stack));
 
    gtk_box_pack_start(GTK_BOX(hbox), gtk_separator_new(GTK_ORIENTATION_VERTICAL), FALSE, FALSE, 0);                               
	gtk_box_pack_start(GTK_BOX(hbox), stack, TRUE, TRUE, 0);
 
	widget = gtk_image_new_from_icon_name("face-angry", GTK_ICON_SIZE_MENU);
	gtk_image_set_pixel_size(GTK_IMAGE(widget), 150);
	gtk_stack_add_named(GTK_STACK(stack), widget, pages[0]);
	gtk_container_child_set(GTK_CONTAINER(stack), widget, "title", pages[0], NULL);
 
	widget = gtk_image_new_from_icon_name("face-sick", GTK_ICON_SIZE_MENU);
	gtk_image_set_pixel_size(GTK_IMAGE(widget), 250);
	gtk_stack_add_named(GTK_STACK(stack), widget, pages[1]);
	gtk_container_child_set(GTK_CONTAINER(stack), widget, "title", pages[1], NULL);
 
	widget = gtk_image_new_from_icon_name("face-laugh", GTK_ICON_SIZE_MENU);
	gtk_image_set_pixel_size(GTK_IMAGE(widget), 350);
	gtk_stack_add_named(GTK_STACK(stack), widget, pages[2]);
	gtk_container_child_set(GTK_CONTAINER(stack), widget, "title", pages[2], NULL);
 
    gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, FALSE, 0);   
    gtk_box_pack_start(GTK_BOX(vbox), bar, FALSE, FALSE, 0);   
    gtk_box_pack_start(GTK_BOX(vbox), gtk_separator_new(GTK_ORIENTATION_HORIZONTAL), FALSE, FALSE, 0);                               
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);                                  
	gtk_container_add(GTK_CONTAINER(window), vbox);                                                             
 
	g_signal_connect_swapped(G_OBJECT(window), "destroy", G_CALLBACK(exit), 0);
	gtk_widget_show_all(window);
	gtk_main();
