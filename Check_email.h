//
// Created by PC on 22-Nov-24.
//

#ifndef CHECK_EMAIL_H
#define CHECK_EMAIL_H
#include <time.h>
#include "Send_Email.h"
#include "hash_password.h"
#include "Bank_information.h"
#include <gtk/gtk.h>

void email_checker(const gchar *first_name, const gchar *last_name, const gchar *cin, const gchar *email, const gchar *birthday, const gchar *password);

#endif //CHECK_EMAIL_H
