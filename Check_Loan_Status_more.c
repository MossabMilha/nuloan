//
// Created by PC on 14-Dec-24.
//

#include "Check_Loan_Status_more.h"

Application Read_application_A(int user_id,int app_id){
  char path[256];
    sprintf(path, "..\\DataBase\\Applications\\user_%d.bin", user_id);
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        perror("Failed to open binary file");
        return (Application){0};
    }
    Application app;
    while (fread(&app, sizeof(Application), 1, file) == 1) {
        if (app.loan_application_id == app_id) {
            fclose(file);
            return app;
        }
    }
    fclose(file);
    return (Application){0};
}
loan_type Read_Loan_A(int id) {
    char path[256];
    sprintf(path, "..\\DataBase\\LOANS\\loan_%d.bin", id);
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        perror("Failed to open binary file");
        return (loan_type){0};
    }
    loan_type loan;
    fread(&loan, sizeof(loan_type), 1, file);
    fclose(file);
    return loan;
}

void app_more_info(GtkApplication *app, gpointer user_data) {
    gpointer *data = (gpointer *)user_data;

    // Retrieve user_id and loan_application_id from the array
    int user_id = GPOINTER_TO_INT(data[0]);
    int loan_application_id = GPOINTER_TO_INT(data[1]);
    GtkWidget *parent = (GtkWidget *)data[2];

    Application loan_application = Read_application_A(user_id, loan_application_id);
    loan_type loan = Read_Loan_A(loan_application.loan_id);


    GtkWidget *window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(window), "Window");
    gtk_window_set_default_size(GTK_WINDOW(window), 350, 600);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    GtkWidget *fix = gtk_fixed_new();

    GtkWidget *title_1 = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title_1), "<span font='15' weight='bold' foreground='#000000'>Loan Information</span>");
    gtk_fixed_put(GTK_FIXED(fix), title_1, 10, 10);

    GtkWidget *text_1 = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(text_1), "<span font='10' weight='bold' foreground='#000000'>Loan Id : </span>");
    gtk_fixed_put(GTK_FIXED(fix), text_1, 10, 50);

    GtkWidget *id_text = gtk_label_new(NULL);
    char id_text_markup[100];
    sprintf(id_text_markup, "<span font='10' foreground='#000000'>%d</span>", loan_application.loan_application_id);
    gtk_label_set_markup(GTK_LABEL(id_text), id_text_markup);
    gtk_fixed_put(GTK_FIXED(fix), id_text, 80, 50);

    GtkWidget *text_2 = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(text_2), "<span font='10' weight='bold' foreground='#000000'>Loan Type : </span>");
    gtk_fixed_put(GTK_FIXED(fix), text_2, 10, 80);

    GtkWidget *laon_type_text = gtk_label_new(NULL);
    char laon_type_markup[100];
    sprintf(laon_type_markup, "<span font='10' foreground='#000000'>%s</span>", loan.loan_title);
    gtk_label_set_markup(GTK_LABEL(laon_type_text), laon_type_markup);
    gtk_fixed_put(GTK_FIXED(fix), laon_type_text, 105, 80);



    GtkWidget *text_3 = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(text_3), "<span font='10' weight='bold' foreground='#000000'>Application Status : </span>");
    gtk_fixed_put(GTK_FIXED(fix), text_3, 10, 110);

    GtkWidget *email_text = gtk_label_new(NULL);
    char email_text_markup[100];
    sprintf(email_text_markup, "<span font='10' foreground='#000000'>%s</span>", loan_application.application_status);
    gtk_label_set_markup(GTK_LABEL(email_text), email_text_markup);
    gtk_fixed_put(GTK_FIXED(fix), email_text, 170, 110);

    GtkWidget *text_4 = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(text_4), "<span font='10' weight='bold' foreground='#000000'> Application Date: </span>");
    gtk_fixed_put(GTK_FIXED(fix), text_4, 10, 140);

    GtkWidget *birthday_text = gtk_label_new(NULL);
    char birthday_text_markup[100];
    sprintf(birthday_text_markup, "<span font='10' foreground='#000000'>%s</span>", loan_application.application_date);
    gtk_label_set_markup(GTK_LABEL(birthday_text), birthday_text_markup);
    gtk_fixed_put(GTK_FIXED(fix), birthday_text, 165, 140);



    GtkWidget *text_5 = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(text_5), "<span font='10' weight='bold' foreground='#000000'>Interest : </span>");
    gtk_fixed_put(GTK_FIXED(fix), text_5, 10, 170);

    GtkWidget *application_id_text = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(application_id_text), g_strdup_printf("<span font='10' foreground='#000000'>%.2f</span>", loan_application.interest_rate));
    gtk_fixed_put(GTK_FIXED(fix), application_id_text, 110, 170);

    GtkWidget *text_6 = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(text_6), "<span font='10' weight='bold' foreground='#000000'>Repayement Start Day : </span>");
    gtk_fixed_put(GTK_FIXED(fix), text_6, 10, 200);

    GtkWidget *loan_type_text = gtk_label_new(NULL);
    char loan_type_text_markup[100];
    sprintf(loan_type_text_markup, "<span font='10' foreground='#000000'>%s</span>", ctime(&loan_application.repayment_start_date));
    gtk_label_set_markup(GTK_LABEL(loan_type_text), loan_type_text_markup);
    gtk_fixed_put(GTK_FIXED(fix), loan_type_text, 110, 220);

    GtkWidget *text_7 = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(text_7), "<span font='10' weight='bold' foreground='#000000'>Loan Duration : </span>");
    gtk_fixed_put(GTK_FIXED(fix), text_7, 10, 250);

    GtkWidget *Amount_Request_text = gtk_label_new(NULL);
    char amount_request_text_markup[100];
    sprintf(amount_request_text_markup, "<span font='10' foreground='#000000'> %d Mounth </span>",loan_application.loan_duration);
    gtk_label_set_markup(GTK_LABEL(Amount_Request_text), amount_request_text_markup);
    gtk_fixed_put(GTK_FIXED(fix), Amount_Request_text, 140, 250);

    GtkWidget *text_8 = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(text_8), "<span font='10' weight='bold' foreground='#000000'>Loan Description  : </span>");
    gtk_fixed_put(GTK_FIXED(fix), text_8, 10, 280);

    GtkWidget *income_text = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(income_text), g_strdup_printf("<span font='10' foreground='#000000'>%s</span>", loan.loan_description));
    gtk_fixed_put(GTK_FIXED(fix), income_text, 10, 300);



    GtkWidget *generate_score_button = gtk_button_new_with_label("Back");
    gtk_fixed_put(GTK_FIXED(fix), generate_score_button, 30, 445);
    gtk_widget_set_size_request(generate_score_button, 300, 20);
    g_signal_connect(generate_score_button, "clicked", G_CALLBACK(Loan_Status_UI), window);



    int id = loan_application.user_id;
    g_object_set_data(G_OBJECT(window), "id", GINT_TO_POINTER(id));


    gtk_window_set_child(GTK_WINDOW(window), fix);
    gtk_window_present(GTK_WINDOW(window));

    // Destroy the parent window (old window)
    gtk_widget_set_visible(parent, FALSE);

    // Free the allocated memory for the 'data' array
    g_free(data);

}