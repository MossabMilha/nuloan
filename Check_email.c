//
// Created by PC on 22-Nov-24.
//
#include <gtk/gtk.h>
#include "Check_email.h"
#include <time.h>
#include "Send_Email.h"
#include "hash_password.h"
#include "Bank_information.h"
User new_user;
extern char global_first_name[50];
extern char global_last_name[50];
extern char global_cin[10];
extern char global_email[50];
extern char global_birthday[11];
extern char global_password[50];


int code_2FA;
int generate_2FA_code() {
    srand(time(NULL));
    return rand() % 900000 + 100000;
}

void send_2FA_Email(const gchar *email) {
    code_2FA = generate_2FA_code();
    char message[1024];
    snprintf(message, sizeof(message), "Dear User,\n"
                                       "Your verification code is: %d\n"
                                       "Please note that this code is valid for only 5 minutes. If you did not request this code, please ignore this email.\n"
                                       "Thank you,\n"
                                       "Your Credit Manager Team", code_2FA);
    send_email(email, "NULOAN Verification", message);
}
void Check(GtkWidget *button, gpointer user_data) {
    GtkWidget *new_window = GTK_WIDGET(user_data);
    GtkWidget *Code = g_object_get_data(G_OBJECT(new_window), "Code");
    const gchar *Code_Text = gtk_editable_get_text(GTK_EDITABLE(Code));


    if (atoi(Code_Text) == code_2FA) {
        new_user.user_id = 1;
        strcpy(new_user.role, "User");
        strcpy(new_user.first_name, global_first_name);
        strcpy(new_user.last_name, global_last_name);
        strcpy(new_user.cin, global_cin);
        strcpy(new_user.email, global_email);
        strcpy(new_user.birthday, global_birthday);
        unsigned char hash[32];
        hash_password(global_password, hash);
        memcpy(new_user.password_hashed, hash, 32);
        new_user.created_at = time(NULL);
        new_user.updated_at = time(NULL);
        strcpy(new_user.status, "Active");
        Bank_Information_open(new_window);

    } else {
        g_print("Incorrect Code\n");
    }

}
void Email_2FA(GtkWidget *button, gpointer user_data) {
    GtkWidget *new_window = GTK_WIDGET(user_data);

    // Retrieve the fixed layout from the new_window object
    GtkWidget *signup_fix = g_object_get_data(G_OBJECT(new_window), "signup_fix");
    // Retrieve the Sent_Message button from the new_window object
    GtkWidget *title = g_object_get_data(G_OBJECT(new_window), "title");

    // Retrieve the Sent_Message button from the new_window object
    GtkWidget *email = g_object_get_data(G_OBJECT(new_window), "Show_Email");
    const gchar *email_to = gtk_label_get_text(GTK_LABEL(email));
    // Retrieve the Sent_Message button from the new_window object
    GtkWidget *Button = g_object_get_data(G_OBJECT(new_window), "Sent Message");
    // Disconnect the existing signal handler
    g_signal_handlers_disconnect_by_func(Button, G_CALLBACK(Email_2FA), new_window);


    // Create the 2FA code entry widget
    GtkWidget *Code_2FA = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(Code_2FA), "Enter the Code you Received");
    gtk_fixed_put(GTK_FIXED(signup_fix), Code_2FA, 50, 300);
    gtk_widget_set_size_request(Code_2FA, 250, 25);
    g_object_set_data(G_OBJECT(new_window), "Code", Code_2FA);

    // Connect the new signal handler
    send_2FA_Email(email_to);
    gtk_button_set_label(GTK_BUTTON(Button), "Submit");
    g_signal_connect(Button, "clicked", G_CALLBACK(Check), new_window);

}

void email_checker(const gchar *first_name, const gchar *last_name, const gchar *cin, const gchar *email, const gchar *birthday, const gchar *password) {

    strcpy(global_first_name, first_name);
    strcpy(global_last_name, last_name);
    strcpy(global_cin, cin);
    strcpy(global_email, email);
    strcpy(global_birthday, birthday);
    strcpy(global_password, password);

    GtkWidget *new_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(new_window), "New Window");
    gtk_window_set_default_size(GTK_WINDOW(new_window), 350, 600);
    gtk_window_set_resizable(GTK_WINDOW(new_window), FALSE);

    // Create a fixed layout for the new window
    GtkWidget *new_fix = gtk_fixed_new();
    gtk_window_set_child(GTK_WINDOW(new_window), new_fix);

    // Add labels to display the information
    GtkWidget *title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title), "<span font='20' weight='bold' foreground='#ff8000'>Verify Your Email</span>");
    gtk_fixed_put(GTK_FIXED(new_fix), title, 10, 50);

    GtkWidget *Show_Email = gtk_label_new(NULL);
    gchar *markup = g_markup_printf_escaped("<span font='20' weight='bold' foreground='#ff8000'>%s</span>", email);
    gtk_label_set_markup(GTK_LABEL(Show_Email), markup);
    g_free(markup);
    gtk_fixed_put(GTK_FIXED(new_fix), Show_Email, 5, 100);

    GtkWidget *Sent_Message = gtk_button_new_with_label("Send Message");
    gtk_fixed_put(GTK_FIXED(new_fix), Sent_Message, 50, 380);
    gtk_widget_set_size_request(Sent_Message, 250, 20);

    // Store the fixed layout and button in the new_window object
    g_object_set_data(G_OBJECT(new_window), "signup_fix", new_fix);
    g_object_set_data(G_OBJECT(new_window), "title", title);
    g_object_set_data(G_OBJECT(new_window), "Show_Email", Show_Email);
    g_object_set_data(G_OBJECT(new_window), "Sent Message", Sent_Message);


    g_signal_connect(Sent_Message, "clicked", G_CALLBACK(Email_2FA), new_window);

    gtk_widget_show(new_window);
}