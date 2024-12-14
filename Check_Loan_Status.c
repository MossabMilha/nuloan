//
// Created by PC on 03-Dec-24.
//

#include "Check_Loan_Status.h"
#include "Active_Member.h"
#include "Structures/Application.h"
#include "Structures/Loans_Types.h"
#include "Check_Loan_Status_more.h"

// Utility function to open a binary file and handle errors
FILE *OpenFile(const char *path, const char *mode) {
    FILE *file = fopen(path, mode);
    if (!file) {
        fprintf(stderr, "Error: Unable to open file %s\n", path);
        exit(EXIT_FAILURE);
    }
    return file;
}

// Reads application information from the file
Application Read_Application_Information(int id, int struct_number) {
    char filename[100];
    sprintf(filename, "..\\DataBase\\Applications\\user_%d.bin", id);
    FILE *file = OpenFile(filename, "rb");

    Application user;
    fseek(file, (struct_number - 1) * sizeof(Application), SEEK_SET);
    fread(&user, sizeof(Application), 1, file);
    fclose(file);
    return user;
}


// Reads loan information from the file
loan_type Read_loan_Information(int id) {
    char filename[100];
    sprintf(filename, "..\\DataBase\\LOANS\\loan_%d.bin", id);
    FILE *file = OpenFile(filename, "rb");

    loan_type loan;
    fread(&loan, sizeof(loan_type), 1, file);
    fclose(file);
    return loan;
}

// Utility function to set markup text for a label
void SetMarkupText(GtkWidget *label, const char *format, ...) {
    char buffer[1000];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    gtk_label_set_markup(GTK_LABEL(label), buffer);
}

// Creates a frame with application details
void Create_User_Frame_1(GtkWidget *parent, const Application app, const loan_type loan, int y_offset, GtkWidget *main_window) {
    gpointer *data = g_malloc(3 * sizeof(gpointer));
    data[0] = GINT_TO_POINTER(app.user_id);
    data[1] = GINT_TO_POINTER(app.loan_application_id);
    data[2] = main_window;
    GtkWidget *frame = gtk_fixed_new();
    gtk_fixed_put(GTK_FIXED(parent), frame, 25, y_offset);

    GtkWidget *frame_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_fixed_put(GTK_FIXED(frame), frame_box, 0, 0);

    // Application ID
    GtkWidget *app_id_label = gtk_label_new(NULL);
    SetMarkupText(app_id_label, "<span font='10' weight='bold'>Application ID: %d</span>", app.loan_application_id);
    gtk_label_set_xalign(GTK_LABEL(app_id_label), 0);

    // Loan Type
    GtkWidget *loan_type_label = gtk_label_new(NULL);
    SetMarkupText(loan_type_label, "<span font='10'>Loan Type: %s</span>", loan.loan_title);
    gtk_label_set_xalign(GTK_LABEL(loan_type_label), 0);


    // Application Status
    GtkWidget *status_label = gtk_label_new(NULL);
    SetMarkupText(status_label, "<span font='10' weight='bold'>Status:</span> <span>%s</span>", app.application_status);
    gtk_label_set_xalign(GTK_LABEL(status_label), 0);

    GtkWidget *more_button = gtk_button_new_with_label("More");
    gtk_widget_set_size_request(more_button, 300, 10);
    g_signal_connect(more_button, "clicked", G_CALLBACK(app_more_info), data);

    gtk_box_append(GTK_BOX(frame_box), app_id_label);
    gtk_box_append(GTK_BOX(frame_box), loan_type_label);
    gtk_box_append(GTK_BOX(frame_box), status_label);
    gtk_box_append(GTK_BOX(frame_box), more_button);

}

// Builds the Loan Status UI
void Loan_Status_UI(GtkWidget *widget, gpointer user_data) {
    GtkWidget *main_window = GTK_WIDGET(user_data);
    int id = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(main_window), "id"));

    GtkWidget *loan_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(loan_window), "Loan Status");
    gtk_window_set_default_size(GTK_WINDOW(loan_window), 350, 700);
    gtk_window_set_resizable(GTK_WINDOW(loan_window), FALSE);

    GtkWidget *fixed_container = gtk_fixed_new();
    gtk_window_set_child(GTK_WINDOW(loan_window), fixed_container);

    // Navigation buttons

    GtkWidget *exit_button = gtk_button_new_with_label("Exit");
    gtk_widget_set_size_request(exit_button, 80, 20);

    gtk_fixed_put(GTK_FIXED(fixed_container), exit_button, 155, 620);

    // g_signal_connect(exit_button, "clicked", G_CALLBACK(Active_Member_Page), loan_window);


    // Display application frames
    for (int i = 0; i < 4; i++) {
        Application app = Read_Application_Information(id, i);
        loan_type loan = Read_loan_Information(app.loan_id);
        Create_User_Frame_1(fixed_container, app, loan, 20 + i * 150,loan_window);
    }

    gtk_widget_set_visible(loan_window, TRUE);
    gtk_widget_set_visible(main_window, FALSE);
}
