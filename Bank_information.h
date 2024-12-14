//
// Created by PC on 23-Nov-24.
//

#ifndef BANK_INFORMATION_H
#define BANK_INFORMATION_H
#include <gtk/gtk.h>
#include "Structures/User.h"
#include "Check_Bank_Information.h"
#include "Structures/Bank_User_info.h"
#include "Save_file.h"
#include "First_Page.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <ctype.h>


void Check_Bank_Information(GtkWidget *widget, gpointer user_data);
void Bank_Information_open(gpointer user_data);
#endif //BANK_INFORMATION_H
