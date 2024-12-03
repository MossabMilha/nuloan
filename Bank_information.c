//
// Created by PC on 23-Nov-24.
//

#include "Bank_information.h"
#include "Check_Bank_Information.h"
#include "Structures/Bank_User_info.h"
#include "Save_file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <ctype.h>

extern User new_user;
Bank_User_info new_bank_user_info;

void Check_Bank_Information(GtkWidget *widget, gpointer user_data) {
    //===========================================================
    GtkWidget *Bank_Information = GTK_WIDGET(user_data);

    GtkWidget *Bank_Name_Entry = GTK_WIDGET(g_object_get_data(G_OBJECT(Bank_Information), "Bank_Name_Entry"));
    GtkEntryBuffer *Bank_Name_Buffer = gtk_entry_get_buffer(GTK_ENTRY(Bank_Name_Entry));
    const gchar *Bank_Name_text = gtk_entry_buffer_get_text(Bank_Name_Buffer);
    GtkWidget *Bank_Name_Button = g_object_get_data(G_OBJECT(Bank_Information), "Bank_Name_Button");

    GtkWidget *Account_Type_Entry = GTK_WIDGET(g_object_get_data(G_OBJECT(Bank_Information), "Account_Type_Entry"));
    GtkEntryBuffer *Account_Type_Buffer = gtk_entry_get_buffer(GTK_ENTRY(Account_Type_Entry));
    const gchar *Account_Type_text = gtk_entry_buffer_get_text(Account_Type_Buffer);
    GtkWidget *Account_Type_Button = g_object_get_data(G_OBJECT(Bank_Information), "Account_Type_Button");

    GtkWidget *Account_Holder_Name_Entry = GTK_WIDGET(g_object_get_data(G_OBJECT(Bank_Information), "Account_Holder_Name_Entry"));
    GtkEntryBuffer *Account_Holder_Name_Buffer = gtk_entry_get_buffer(GTK_ENTRY(Account_Holder_Name_Entry));
    const gchar *Account_Holder_Name_text = gtk_entry_buffer_get_text(Account_Holder_Name_Buffer);
    GtkWidget *Account_Holder_Name_Button = g_object_get_data(G_OBJECT(Bank_Information), "Account_Holder_Name_Button");

    GtkWidget *Account_Number_Entry = GTK_WIDGET(g_object_get_data(G_OBJECT(Bank_Information), "Account_Number_Entry"));
    GtkEntryBuffer *Account_Number_Buffer = gtk_entry_get_buffer(GTK_ENTRY(Account_Number_Entry));
    const gchar *Account_Number_text = gtk_entry_buffer_get_text(Account_Number_Buffer);
    GtkWidget *Account_Number_Button = g_object_get_data(G_OBJECT(Bank_Information), "Account_Number_Button");

    if(is_empty(Bank_Name_text) || is_empty(Account_Type_text) || is_empty(Account_Holder_Name_text) || is_empty(Account_Number_text)){
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(Bank_Information), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "All Fields are Required");
        g_signal_connect(dialog, "response", G_CALLBACK(gtk_window_close), NULL);
        gtk_widget_show(dialog);
    }else if(is_Bank_Name_Valid(Bank_Name_text) && is_Account_Holder_Name_Valid(Account_Holder_Name_text)&&is_Account_Number_Valid(Account_Number_text, Bank_Name_text)) {
        gtk_widget_set_visible(Bank_Name_Button, FALSE);
        gtk_widget_set_visible(Account_Holder_Name_Button, FALSE);
        gtk_widget_set_visible(Account_Number_Button, FALSE);
        new_bank_user_info.bank_id = new_user.user_id;
        printf("debug 2 : %d\n", new_user.user_id);
        new_bank_user_info.user_id = new_user.user_id;
        strcpy(new_bank_user_info.bank_name, Bank_Name_text);
        strcpy(new_bank_user_info.account_type, Account_Type_text);
        strcpy(new_bank_user_info.account_holder_name, Account_Holder_Name_text);
        strcpy(new_bank_user_info.account_number, Account_Number_text);
        time(&new_bank_user_info.created_at);
        time(&new_bank_user_info.updated_at);
        if(Save_Signup_Information(new_user, new_bank_user_info)) {
            printf("the Bank Information is saved successfully\n");
        }



    }else {
        if (is_Bank_Name_Valid(Bank_Name_text)) {
            gtk_widget_set_visible(Bank_Name_Button, FALSE);
        }else {
            gtk_widget_set_visible(Bank_Name_Button, TRUE);
            gtk_widget_set_tooltip_text(Bank_Name_Button, "Invalid Bank Name Or We Don't Support This Bank Yet");

        }
        if (!is_empty(Account_Type_text)) {

        }else {
            printf("The Field of Account Type is Empty\n");
        }
        if (is_Account_Holder_Name_Valid(Account_Holder_Name_text)) {
            gtk_widget_set_visible(Account_Holder_Name_Button, FALSE);
        }else {
            gtk_widget_set_visible(Account_Holder_Name_Button, TRUE);
            gtk_widget_set_tooltip_text(Account_Holder_Name_Button, "Invalid Account Holder Name It Should Contain Only Letters");
        }

        if (is_Account_Number_Valid(Account_Number_text, Bank_Name_text)) {
            gtk_widget_set_visible(Account_Number_Button, FALSE);
        }else {
            gtk_widget_set_visible(Account_Number_Button, TRUE);
            gtk_widget_set_tooltip_text(Account_Number_Button, "Invalid Account Number");
        }
    }
}
void Bank_Information_open(gpointer user_data) {

    GtkWidget *main_window = GTK_WIDGET(user_data);
    // Create the Bank Information window
    GtkWidget *Bank_Information = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(Bank_Information), "Bank Information");
    gtk_window_set_default_size(GTK_WINDOW(Bank_Information), 350, 600);
    gtk_window_set_resizable(GTK_WINDOW(Bank_Information), FALSE);

    // Create a fixed layout for the Bank Information window
    GtkWidget *Bank_Information_Fix = gtk_fixed_new();
    gtk_window_set_child(GTK_WINDOW(Bank_Information), Bank_Information_Fix);

    // Bank Name entry
    GtkWidget *Bank_Name_Entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(Bank_Name_Entry), "Bank Name");
    gtk_fixed_put(GTK_FIXED(Bank_Information_Fix), Bank_Name_Entry, 45, 200);
    gtk_widget_set_size_request(Bank_Name_Entry, 250, 25);
    //Bank Name image
    GtkWidget *Bank_Name_Button = gtk_button_new();
    GtkWidget *Bank_Name_image = gtk_image_new_from_file("..\\images\\info_red.png");
    gtk_button_set_child(GTK_BUTTON(Bank_Name_Button), Bank_Name_image);
    gtk_fixed_put(GTK_FIXED(Bank_Information_Fix), Bank_Name_Button, 300, 200);
    gtk_widget_set_size_request(Bank_Name_Button, 20, 20);
    gtk_widget_set_visible(Bank_Name_Button, FALSE);
    gtk_widget_set_sensitive(Bank_Name_Button, FALSE);


    // Account Type entry
    GtkWidget *Account_Type_Entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(Account_Type_Entry), "Bank Type");
    gtk_fixed_put(GTK_FIXED(Bank_Information_Fix), Account_Type_Entry, 45, 240);
    gtk_widget_set_size_request(Account_Type_Entry, 250, 25);
    //Bank Name image
    GtkWidget *Account_Type_Button = gtk_button_new();
    GtkWidget *Account_Type_image = gtk_image_new_from_file("..\\images\\info_red.png");
    gtk_button_set_child(GTK_BUTTON(Account_Type_Button), Account_Type_image);
    gtk_fixed_put(GTK_FIXED(Bank_Information_Fix), Account_Type_Button, 300, 240);
    gtk_widget_set_size_request(Account_Type_Button, 20, 20);
    gtk_widget_set_visible(Account_Type_Button, FALSE);
    gtk_widget_set_sensitive(Account_Type_Button, FALSE);

    // Account Holder Name entry
    GtkWidget *Account_Holder_Name_Entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(Account_Holder_Name_Entry), "Account Holder Name");
    gtk_fixed_put(GTK_FIXED(Bank_Information_Fix), Account_Holder_Name_Entry, 45, 280);
    gtk_widget_set_size_request(Account_Holder_Name_Entry, 250, 25);
    //Bank Name image
    GtkWidget *Account_Holder_Name_Button = gtk_button_new();
    GtkWidget *Account_Holder_Name_image = gtk_image_new_from_file("..\\images\\info_red.png");
    gtk_button_set_child(GTK_BUTTON(Account_Holder_Name_Button), Account_Holder_Name_image);
    gtk_fixed_put(GTK_FIXED(Bank_Information_Fix), Account_Holder_Name_Button, 300, 280);
    gtk_widget_set_size_request(Account_Holder_Name_Button, 20, 20);
    gtk_widget_set_visible(Account_Holder_Name_Button, FALSE);
    gtk_widget_set_sensitive(Account_Holder_Name_Button, FALSE);

    // Account Number entry
    GtkWidget *Account_Number_Entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(Account_Number_Entry), "Account Number");
    gtk_fixed_put(GTK_FIXED(Bank_Information_Fix), Account_Number_Entry, 45, 320);
    gtk_widget_set_size_request(Account_Number_Entry, 250, 25);
    //Bank Name image
    GtkWidget *Account_Number_Button = gtk_button_new();
    GtkWidget *Account_Number_image = gtk_image_new_from_file("..\\images\\info_red.png");
    gtk_button_set_child(GTK_BUTTON(Account_Number_Button), Account_Number_image);
    gtk_fixed_put(GTK_FIXED(Bank_Information_Fix), Account_Number_Button, 300, 320);
    gtk_widget_set_size_request(Account_Number_Button, 20, 20);
    gtk_widget_set_visible(Account_Number_Button, FALSE);
    gtk_widget_set_sensitive(Account_Number_Button, FALSE);

    g_object_set_data(G_OBJECT(Bank_Information), "Bank_Information_Fix", Bank_Information_Fix);
    g_object_set_data(G_OBJECT(Bank_Information), "Bank_Name_Entry", Bank_Name_Entry);
    g_object_set_data(G_OBJECT(Bank_Information), "Bank_Name_Button", Bank_Name_Button);
    g_object_set_data(G_OBJECT(Bank_Information), "Account_Type_Entry", Account_Type_Entry);
    g_object_set_data(G_OBJECT(Bank_Information), "Account_Type_Button", Account_Type_Button);
    g_object_set_data(G_OBJECT(Bank_Information), "Account_Holder_Name_Entry", Account_Holder_Name_Entry);
    g_object_set_data(G_OBJECT(Bank_Information), "Account_Holder_Name_Button", Account_Holder_Name_Button);
    g_object_set_data(G_OBJECT(Bank_Information), "Account_Number_Entry", Account_Number_Entry);
    g_object_set_data(G_OBJECT(Bank_Information), "Account_Number_Button", Account_Number_Button);

    // Create Submit Button
    GtkWidget *Submit = gtk_button_new_with_label("Submit");
    gtk_fixed_put(GTK_FIXED(Bank_Information_Fix), Submit, 45, 360);
    gtk_widget_set_size_request(Submit, 250, 20);
    g_signal_connect(Submit, "clicked", G_CALLBACK(Check_Bank_Information), Bank_Information);

    gtk_window_set_child(GTK_WINDOW(Bank_Information), Bank_Information_Fix);

    // Show all widgets
    gtk_window_present(GTK_WINDOW(Bank_Information));
    gtk_widget_hide(main_window);
}
