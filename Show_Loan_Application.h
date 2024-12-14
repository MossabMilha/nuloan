//
// Created by PC on 07-Dec-24.
//

#ifndef SHOW_LOAN_APPLICATION_H
#define SHOW_LOAN_APPLICATION_H
#include <gtk/gtk.h>
#include <gtk/gtk.h>
#include <dirent.h>
#include <sys/stat.h>
#include "Structures/Application.h"  // Include the struct for Loan_Apply
#include "Structures/User.h"
#include "Structures/Loans_Types.h"
#include "Approve_Reject_Application.h"
#include "Super_Admin.h"
void Check_Applications(GtkWidget *widget, gpointer data);
#endif //SHOW_LOAN_APPLICATION_H
