#include <stdio.h>
#include <gtk/gtk.h>
#include "SignUp.h"
#include "Check_Signup_Information.h"
#include "Show_Users.h"

void Admin_Page(GtkButton *button, gpointer user_data) {
    GtkWidget *main_window = GTK_WIDGET(user_data);

    // Create a new Signup window
    GtkWidget *Admin_Page = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(Admin_Page), "Admin Page");
    gtk_window_set_default_size(GTK_WINDOW(Admin_Page), 1800, 700);
    gtk_window_set_resizable(GTK_WINDOW(Admin_Page), FALSE);

    // Create a fixed layout for the Bank Information window
    GtkWidget *Admin_Page_Fix = gtk_fixed_new();
    gtk_widget_set_size_request(Admin_Page_Fix, 400, 600);

    // Create a fixed layout for the Admin OutPut
    GtkWidget *Admin_OutPut_Fix = gtk_fixed_new();
    gtk_widget_set_size_request(Admin_OutPut_Fix, 120, 600);

    // Create Show Users_information button
    GtkWidget *Show_User = gtk_button_new_with_label("Show Users");
    gtk_fixed_put(GTK_FIXED(Admin_Page_Fix), Show_User, 75, 100);
    gtk_widget_set_size_request(Show_User, 250, 20);



    // Add the fixed layouts to a container
    GtkWidget *container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_append(GTK_BOX(container), Admin_Page_Fix);
    gtk_box_append(GTK_BOX(container), Admin_OutPut_Fix);
    gtk_window_set_child(GTK_WINDOW(Admin_Page), container);

    int Page = 1;
    g_object_set_data(G_OBJECT(Admin_Page), "Show_User", Show_User);
    g_object_set_data(G_OBJECT(Admin_Page), "Admin_Page_Fix", Admin_Page_Fix);
    g_object_set_data(G_OBJECT(Admin_Page), "Admin_OutPut_Fix", Admin_OutPut_Fix);
    g_object_set_data(G_OBJECT(Admin_Page), "Page", GINT_TO_POINTER(Page));

    g_signal_connect(Show_User, "clicked", G_CALLBACK(Check_User), Admin_Page);
    // Show all widgets
    gtk_window_present(GTK_WINDOW(Admin_Page));

    gtk_widget_set_visible(Admin_Page, TRUE);
    gtk_widget_set_visible(main_window, FALSE);
}