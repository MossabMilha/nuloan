

#ifndef CHECK_USER_INFORMATION_H
#define CHECK_USER_INFORMATION_H
#define MIN_PASSWORD_LENGTH 8
#define SPECIAL_CHARACTERS "!@#$%^&*()-_=+[]{}|;:'\",.<>?/\\"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <regex.h>
#include <time.h>
#include <gcrypt.h>
void check_first_name(const char *first_name, GtkWidget *first_name_button);
void check_last_name(const char *last_name, GtkWidget *last_name_button);
void check_CIN(const char *cin, GtkWidget *cin_button);
void check_email(const char *email,GtkWidget *gmail_button);
void check_birthday(const char *birthday, GtkWidget *birthday_button);
char* check_password(const gchar *password, const gchar *confirm_password, const gchar *first_name, const gchar *last_name, GtkWidget *password_button);

#endif //CHECK_USER_INFORMATION_H
