//
// Created by PC on 01-Dec-24.
//

#include "Login_Process_Handle_Window.h"
#include "hash_password.h"
#include "Structures/User.h"
#include "Super_Admin.h"
#include "Active_Member.h"
#include <string.h>


void check_login(GtkWidget *widget, gpointer user_data) {

    GtkWidget *main_window = GTK_WIDGET(user_data);
    GtkWidget *id_entry = g_object_get_data(G_OBJECT(main_window), "id_entry");
    GtkEntryBuffer *id_buffer = gtk_entry_get_buffer(GTK_ENTRY(id_entry));
    const gchar *id = gtk_entry_buffer_get_text(id_buffer);

    GtkWidget *password_entry = g_object_get_data(G_OBJECT(main_window), "password_entry");
    GtkEntryBuffer *password_buffer = gtk_entry_get_buffer(GTK_ENTRY(password_entry));
    const gchar *password = gtk_entry_buffer_get_text(password_buffer);
    unsigned char hashed_password[32];
    hash_password(password, hashed_password);

    char path[100];
    User user;
    snprintf(path, sizeof(path), "..\\DataBase\\Users\\user_%s.bin", id);
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        printf("User not found\n");
        return;
    }
    fread(&user, sizeof(User), 1, file);
    fclose(file);
    if (memcmp(user.password_hashed, hashed_password, 32) == 0) {

        printf("%s\n",user.role);
        if(strcmp(user.role, "super admin") == 0) {
            printf("Welcome Super Admin\n");
            Active_Member_Page(GTK_BUTTON(widget), user_data);
        } else if(strcmp(user.role, "admin") == 0) {
            printf("Welcome Admin \n");
        } else if(strcmp(user.role, "user") == 0) {
            if (strcmp(user.status , "Active") == 0) {
                Active_Member_Page(GTK_BUTTON(widget), user_data);
            } else {
                printf("User is not active\n");
            }
        }
    } else {
        
        printf("Incorrect password\n");
    }


}
