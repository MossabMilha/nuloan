#include <stdio.h>
#include <gtk/gtk.h>
#include "SignUp.h"
#include "Login.h"

char global_first_name[50];
char global_last_name[50];
char global_cin[10];
char global_email[50];
char global_birthday[11];
char global_password[50];





int global_2FA_code;

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *fix;

    // Create a new window for the application
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Window");
    gtk_window_set_default_size(GTK_WINDOW(window), 350, 600);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    // Create a CSS provider and set the background color
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(provider, "window { background-color: #FFFFFF; }");

    // Attach the CSS provider to the window's style context
    gtk_style_context_add_provider_for_display(gdk_display_get_default(),
        GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Title information
    GtkWidget *title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title), "<span font='20' weight='bold' foreground='#ff8000'>\t   Welcome \n\t\t  To\n   Your Credit Manager</span>");

    fix = gtk_fixed_new();
    gtk_fixed_put(GTK_FIXED(fix), title, 40, 50);

    // Login button
    GtkWidget *Login_Button = gtk_button_new_with_label("Login");
    gtk_fixed_put(GTK_FIXED(fix), Login_Button, 65, 400);
    gtk_widget_set_size_request(Login_Button, 100, 50);

    // Connect the clicked signal of the login button, passing the main window as user_data
    g_signal_connect(Login_Button, "clicked", G_CALLBACK(Login_UI), window);

    // Set the child widget

    // SignUp button
    GtkWidget *SignUp_Button = gtk_button_new_with_label("SignUp");
    gtk_fixed_put(GTK_FIXED(fix), SignUp_Button, 185, 400);
    gtk_widget_set_size_request(SignUp_Button, 100, 50);

    // Connect the clicked signal of the signup button, passing the main window as user_data
    g_signal_connect(SignUp_Button, "clicked", G_CALLBACK(Sign_Up_UI), window);

    gtk_window_set_child(GTK_WINDOW(window), fix);

    // Present the main window
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}