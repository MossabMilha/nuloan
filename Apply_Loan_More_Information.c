#include "Apply_Loan_More_Information.h"
#include "Apply_Loan.h"
#include "Apply_Loan_fianl.h"
#include "Structures/Loans_Types.h"
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

loan_type loan_info[6];

// Function to read loan information from binary files
void read_loan_info_2() {
    for (int i = 0; i < 6; i++) {
        char path[100];
        snprintf(path, sizeof(path), "..\\DataBase\\Loans\\loan_%d.bin", i + 1);
        FILE *file = fopen(path, "rb");
        if (file == NULL) {
            fprintf(stderr, "Could not open loan_%d.bin for reading\n", i + 1);
            exit(1);
        }
        fread(&loan_info[i], sizeof(loan_type), 1, file);
        fclose(file);
    }
}

// Helper function to add a labeled field to the GTK Fixed layout
void add_field_to_fixed(GtkWidget *fixed, const char *title, const char *value, int x, int y) {
    GtkWidget *label_title = gtk_label_new("");
    char text[500];
    sprintf(text, "<span font='12' weight='bold' foreground='#000000'>%s</span>", title);
    gtk_label_set_markup(GTK_LABEL(label_title), text);
    gtk_fixed_put(GTK_FIXED(fixed), label_title, x, y);

    GtkWidget *label_value = gtk_label_new("");
    sprintf(text, "<span font='12' foreground='#000000'>%s</span>", value);
    gtk_label_set_markup(GTK_LABEL(label_value), text);
    gtk_fixed_put(GTK_FIXED(fixed), label_value, x + 120, y); // Adjust spacing for value
}

// Function to display the Apply Loan Page 2 UI
void Apply_Loan_Page_2_UI(GtkWidget *widget, gpointer user_data) {
    read_loan_info_2();
    int *tab = (int *)user_data;
    int id = tab[0];  // User ID
    int i = tab[1];   // Loan ID


    // Check for valid index
    if (i < 0 || i >= 6) {
        fprintf(stderr, "Invalid loan index: %d\n", i);
        return;
    }

    // Create a new window
    GtkWidget *Apply_Loan_Page2_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(Apply_Loan_Page2_window), "Apply To Loan");
    gtk_window_set_default_size(GTK_WINDOW(Apply_Loan_Page2_window), 350, 600);
    gtk_window_set_resizable(GTK_WINDOW(Apply_Loan_Page2_window), FALSE);

    GtkWidget *Apply_Loan_fix = gtk_fixed_new();
    gtk_window_set_child(GTK_WINDOW(Apply_Loan_Page2_window), Apply_Loan_fix);

    // Add Loan Title
    GtkWidget *label = gtk_label_new("");
    char text[500];
    sprintf(text, "<span font='15' weight='bold' foreground='#000000'>%s</span>", loan_info[i].loan_title);
    gtk_label_set_markup(GTK_LABEL(label), text);
    gtk_fixed_put(GTK_FIXED(Apply_Loan_fix), label, 90, 20);

    // Add loan details
    char value[50];
    sprintf(value, "%.2f", loan_info[i].min_amount);
    add_field_to_fixed(Apply_Loan_fix, "• Min Amount:", value, 20, 80);

    sprintf(value, "%.2f", loan_info[i].max_amount);
    add_field_to_fixed(Apply_Loan_fix, "• Max Amount:", value, 20, 120);

    sprintf(value, "%d", loan_info[i].min_duration);
    add_field_to_fixed(Apply_Loan_fix, "• Min Duration:", value, 20, 160);

    sprintf(value, "%.2f", *loan_info[i].interest_rate);
    add_field_to_fixed(Apply_Loan_fix, "• Interest:", value, 20, 200);

    // Add long description
    GtkWidget *long_description_label = gtk_label_new("");
    sprintf(text, "<span font='12' weight='bold' foreground='#000000'>• Description:</span>");
    gtk_label_set_markup(GTK_LABEL(long_description_label), text);
    gtk_fixed_put(GTK_FIXED(Apply_Loan_fix), long_description_label, 20, 240);

    GtkWidget *long_description_label_1 = gtk_label_new("");
    sprintf(text, "<span font='12' foreground='#000000'>%s</span>", loan_info[i].loan_description);
    gtk_label_set_markup(GTK_LABEL(long_description_label_1), text);
    gtk_fixed_put(GTK_FIXED(Apply_Loan_fix), long_description_label_1, 20, 280);

    // Add Apply button
    GtkWidget *apply_button = gtk_button_new_with_label("Apply");
    gtk_widget_set_size_request(apply_button, 300, 30);
    gtk_fixed_put(GTK_FIXED(Apply_Loan_fix), apply_button, 25, 450);

    // Store user_id and loan_id in the apply_button widget
    g_object_set_data(G_OBJECT(Apply_Loan_Page2_window), "user_id", GINT_TO_POINTER(id));
    g_object_set_data(G_OBJECT(Apply_Loan_Page2_window), "loan_id", GINT_TO_POINTER(i));

    g_signal_connect(apply_button, "clicked", G_CALLBACK(Final_Apply), Apply_Loan_Page2_window);

    // Add Go Back button
    GtkWidget *Back_button = gtk_button_new_with_label("Go Back");
    gtk_widget_set_size_request(Back_button, 300, 30);
    gtk_fixed_put(GTK_FIXED(Apply_Loan_fix), Back_button, 25, 500);

    g_signal_connect(Back_button, "clicked", G_CALLBACK(gtk_window_destroy), Apply_Loan_Page2_window);

    // Display the new window and hide the previous widget
    gtk_widget_set_visible(Apply_Loan_Page2_window, TRUE);
    gtk_widget_set_visible(widget, FALSE);
}
