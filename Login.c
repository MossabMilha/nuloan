#include <stdio.h>
#include "Login.h"
#include "Login_Process_Handle_Window.h"

void Login_UI(GtkWidget *widget, gpointer user_data) {
    GtkWidget *main_window = GTK_WIDGET(user_data);

    // Create a new Signup window
    GtkWidget *login_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(login_window), "Login");
    gtk_window_set_default_size(GTK_WINDOW(login_window), 350, 600);
    gtk_window_set_resizable(GTK_WINDOW(login_window), FALSE);

    GtkWidget *login_fix = gtk_fixed_new();
    gtk_window_set_child(GTK_WINDOW(login_window), login_fix);

    // ID entry
    GtkWidget *id_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(id_entry), "ID");
    gtk_fixed_put(GTK_FIXED(login_fix), id_entry, 50, 200);
    gtk_widget_set_size_request(id_entry, 250, 20);

    // Password entry
    GtkWidget *password_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(password_entry), "Password");
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);
    gtk_entry_set_invisible_char(GTK_ENTRY(password_entry), '*');
    gtk_fixed_put(GTK_FIXED(login_fix), password_entry, 50, 240);
    gtk_widget_set_size_request(password_entry, 250, 20);

    // Login button
    GtkWidget *login_button = gtk_button_new_with_label("Login");
    gtk_fixed_put(GTK_FIXED(login_fix), login_button, 50, 280);
    gtk_widget_set_size_request(login_button, 250, 20);

    g_object_set_data(G_OBJECT(login_window), "id_entry", id_entry);
    g_object_set_data(G_OBJECT(login_window), "password_entry", password_entry);


    g_signal_connect(login_button, "clicked", G_CALLBACK(check_login), login_window);
    gtk_window_set_child(GTK_WINDOW(login_window), login_fix);

    gtk_widget_set_visible(login_window, TRUE);
    gtk_widget_set_visible(main_window, FALSE);


}