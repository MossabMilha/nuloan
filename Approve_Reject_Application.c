//
// Created by PC on 09-Dec-24.
//

#include "Approve_Reject_Application.h"
#include "Structures/User.h"
#include "Structures/Application.h"
#include "Structures/Loans_Types.h"
User user;
Application loan_application;
loan_type loan;
int loan_application_id;
float suggest_interest_rate(int score, loan_type loan) {
    // Higher scores get lower interest rates within the loan's range
    float interest_rate;
    if (score > 750) {
        interest_rate = loan.interest_rate[0]; // Best rate for excellent credit
    } else if (score > 650) {
        interest_rate = (loan.interest_rate[0] + loan.interest_rate[1]) / 2; // Mid-point rate for good credit
    } else {
        interest_rate = loan.interest_rate[1]; // Highest rate for fair credit
    }
    return interest_rate;
}

int calculate_credit_score(User user, Application loan_application, loan_type loan) {
    int score = 300; // Initial minimum score

    // Criterion 1: Loan Duration vs Loan Type
    if (loan_application.loan_duration >= loan.min_duration && loan_application.loan_duration <= loan.max_duration) {
        score += 30; // Duration aligns with loan type requirements
    } else {
        score -= 30; // Duration mismatch (assuming this case is possible within bounds)
    }

    // Criterion 2: User Account Age
    time_t current_time = time(NULL);
    double account_age_years = difftime(current_time, user.created_at) / (365.0 * 24.0 * 60.0 * 60.0);
    if (account_age_years > 5) {
        score += 100; // Long account tenure indicates reliability
    } else if (account_age_years > 2) {
        score += 50; // Moderate account tenure
    } else {
        score -= 20; // Recent account, less reliability
    }

    // Criterion 3: User Status
    if (strcmp(user.status, "active") == 0) {
        score += 50; // Active status indicates engagement and reliability
    } else {
        score -= 50; // Inactive or unknown status
    }

    // Criterion 4: Application Status
    if (strcmp(loan_application.application_status, "Approved") == 0) {
        score += 30; // Bonus for previously approved applications
    } else if (strcmp(loan_application.application_status, "Declined") == 0) {
        score -= 50; // Penalty for declined applications
    } else if (strcmp(loan_application.application_status, "In Progress") == 0) {
        score += 10; // Small bonus for active applications
    }

    // Criterion 5: Timeliness of Repayment Start Date
    if (difftime(current_time, loan_application.repayment_start_date) < 0) {
        score += 10; // Future repayment start indicates planning
    } else {
        score -= 20; // Late or already started repayment indicates risk
    }

    // Cap the score between 300 and 850
    if (score > 850) {
        score = 850;
    } else if (score < 300) {
        score = 300;
    }

    // Suggest interest rate based on score

    return score;
}
Application Read_application(int id) {
    char bin_file_path[256];
    snprintf(bin_file_path, sizeof(bin_file_path), "..\\DataBase\\Application_Not_Checked\\application_%d.bin", id);
    FILE *bin_file = fopen(bin_file_path, "rb");
    if (bin_file == NULL) {
        perror("Failed to open binary file");
        return (Application){0}; // Return an empty User struct
    }
    Application app;
    fread(&app, sizeof(app), 1, bin_file);
    fclose(bin_file);
    return app;
}
User Read_User_1(int id) {
    char bin_file_path[256];
    snprintf(bin_file_path, sizeof(bin_file_path), "..\\DataBase\\Users\\user_%d.bin", id);
    FILE *bin_file = fopen(bin_file_path, "rb");
    if (bin_file == NULL) {
        perror("Failed to open binary file");
        return (User){0}; // Return an empty User struct
    }
    User user;
    fread(&user, sizeof(user), 1, bin_file);
    fclose(bin_file);
    return user;
}
loan_type Read_Loan_1(int id) {
    char bin_file_path[256];
    snprintf(bin_file_path, sizeof(bin_file_path), "..\\DataBase\\LOANS\\loan_%d.bin", id);
    FILE *bin_file = fopen(bin_file_path, "rb");
    if (bin_file == NULL) {
        perror("Failed to open binary file");
        return (loan_type){0}; // Return an empty User struct
    }
    loan_type loan;
    fread(&loan, sizeof(loan), 1, bin_file);
    fclose(bin_file);
    return loan;
}
void final_step(GtkWidget *app, gpointer user_data) {
    GtkWidget *window = GTK_WIDGET(user_data);
    GtkWidget *fix = g_object_get_data(G_OBJECT(window), "fix");
    GtkWidget *generate_score_button = g_object_get_data(G_OBJECT(window), "generate_score_button");
    gtk_button_set_label(GTK_BUTTON(generate_score_button), "Approve With my Score");
    GtkWidget *Approve_button = g_object_get_data(G_OBJECT(window), "Approve_button");
    GtkWidget *Reject_button = g_object_get_data(G_OBJECT(window), "Reject_button");

    if (GTK_IS_WIDGET(Approve_button)) {
        gtk_widget_set_visible(Approve_button, TRUE);
    }
    if (GTK_IS_WIDGET(Reject_button)) {
        gtk_widget_set_visible(Reject_button, TRUE);
    }

    int score = calculate_credit_score(user, loan_application, loan);
    float interest_rate = suggest_interest_rate(score, loan);
    GtkWidget *text_10 = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(text_10), g_strdup_printf("<span font='10' weight='bold' foreground='red'>Score : %d</span>", score));
    gtk_fixed_put(GTK_FIXED(fix), text_10, 35, 420);



    GtkWidget *text_11 = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(text_11), g_strdup_printf("<span font='10' weight='bold' foreground='red'>Interest Suggest : %.2f%%</span>", interest_rate));
    gtk_fixed_put(GTK_FIXED(fix), text_11, 150, 420);
}

// Activate callback function
void Approve_Activate(GtkApplication *app, gpointer user_data) {

    loan_application_id = GPOINTER_TO_INT(user_data); // Access the loan_application_id
    printf("Loan Application ID: %d\n", loan_application_id);

    loan_application = Read_application(loan_application_id);
    user = Read_User_1(loan_application.user_id);
    loan = Read_Loan_1(loan_application.loan_id);


    // Create a new window for the application
    GtkWidget *window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(window), "Window");
    gtk_window_set_default_size(GTK_WINDOW(window), 350, 600);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    // Create a fixed container
    GtkWidget *fix = gtk_fixed_new();

    // Title 1: User Information
    GtkWidget *title_1 = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title_1), "<span font='15' weight='bold' foreground='#000000'>User Information</span>");
    gtk_fixed_put(GTK_FIXED(fix), title_1, 10, 10);

    // User Information Fields
    GtkWidget *text_1 = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(text_1), "<span font='10' weight='bold' foreground='#000000'>User Id : </span>");
    gtk_fixed_put(GTK_FIXED(fix), text_1, 10, 50);

    GtkWidget *id_text = gtk_label_new(NULL);
    char id_text_markup[100];
    sprintf(id_text_markup, "<span font='10' foreground='#000000'>%d</span>", user.user_id);
    gtk_label_set_markup(GTK_LABEL(id_text), id_text_markup);
    gtk_fixed_put(GTK_FIXED(fix), id_text, 70, 50);

    GtkWidget *text_2 = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(text_2), "<span font='10' weight='bold' foreground='#000000'>Full Name : </span>");
    gtk_fixed_put(GTK_FIXED(fix), text_2, 10, 80);

    GtkWidget *Full_Name_text = gtk_label_new(NULL);
    char full_name_text_markup[200];
    sprintf(full_name_text_markup, "<span font='10' foreground='#000000'>%s %s</span>", user.first_name, user.last_name);
    gtk_label_set_markup(GTK_LABEL(Full_Name_text), full_name_text_markup);
    gtk_fixed_put(GTK_FIXED(fix), Full_Name_text, 90, 80);;

    GtkWidget *text_3 = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(text_3), "<span font='10' weight='bold' foreground='#000000'>Email : </span>");
    gtk_fixed_put(GTK_FIXED(fix), text_3, 10, 110);

    GtkWidget *email_text = gtk_label_new(NULL);
    char email_text_markup[100];
    sprintf(email_text_markup, "<span font='10' foreground='#000000'>%s</span>", user.email);
    gtk_label_set_markup(GTK_LABEL(email_text), email_text_markup);
    gtk_fixed_put(GTK_FIXED(fix), email_text, 60, 110);

    GtkWidget *text_4 = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(text_4), "<span font='10' weight='bold' foreground='#000000'>Birthday : </span>");
    gtk_fixed_put(GTK_FIXED(fix), text_4, 10, 140);

    GtkWidget *birthday_text = gtk_label_new(NULL);
    char birthday_text_markup[100];
    sprintf(birthday_text_markup, "<span font='10' foreground='#000000'>%s</span>", user.birthday);
    gtk_label_set_markup(GTK_LABEL(birthday_text), birthday_text_markup);
    gtk_fixed_put(GTK_FIXED(fix), birthday_text, 80, 140);gtk_fixed_put(GTK_FIXED(fix), birthday_text, 80, 140);

    // Title 2: Application Information
    GtkWidget *title_2 = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title_2), "<span font='15' weight='bold' foreground='#000000'>Application Information</span>");
    gtk_fixed_put(GTK_FIXED(fix), title_2, 10, 180);

    // Application Information Fields
    GtkWidget *text_5 = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(text_5), "<span font='10' weight='bold' foreground='#000000'>Application Id : </span>");
    gtk_fixed_put(GTK_FIXED(fix), text_5, 10, 220);

    GtkWidget *application_id_text = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(application_id_text), g_strdup_printf("<span font='10' foreground='#000000'>%d</span>", loan_application.loan_application_id));
    gtk_fixed_put(GTK_FIXED(fix), application_id_text, 110, 220);

    GtkWidget *text_6 = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(text_6), "<span font='10' weight='bold' foreground='#000000'>Loan Type : </span>");
    gtk_fixed_put(GTK_FIXED(fix), text_6, 10, 250);

    GtkWidget *loan_type_text = gtk_label_new(NULL);
    char loan_type_text_markup[100];
    sprintf(loan_type_text_markup, "<span font='10' foreground='#000000'>%s</span>", loan.loan_title);
    gtk_label_set_markup(GTK_LABEL(loan_type_text), loan_type_text_markup);
    gtk_fixed_put(GTK_FIXED(fix), loan_type_text, 90, 250);

    GtkWidget *text_7 = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(text_7), "<span font='10' weight='bold' foreground='#000000'>Amount Request : </span>");
    gtk_fixed_put(GTK_FIXED(fix), text_7, 10, 280);

    GtkWidget *Amount_Request_text = gtk_label_new(NULL);
    char amount_request_text_markup[100];
    sprintf(amount_request_text_markup, "<span font='10' foreground='#000000'> %.2f MAD </span>", loan_application.amount_requested);
    gtk_label_set_markup(GTK_LABEL(Amount_Request_text), amount_request_text_markup);
    gtk_fixed_put(GTK_FIXED(fix), Amount_Request_text, 120, 280);

    GtkWidget *text_8 = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(text_8), "<span font='10' weight='bold' foreground='#000000'>Income : </span>");
    gtk_fixed_put(GTK_FIXED(fix), text_8, 10, 310);

    GtkWidget *income_text = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(income_text), g_strdup_printf("<span font='10' foreground='#000000'> %d MAD</span>", loan_application.income));
    gtk_fixed_put(GTK_FIXED(fix), income_text, 70, 310);

    GtkWidget *text_9 = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(text_9), "<span font='10' weight='bold' foreground='#000000'>Loan Duration : </span>");
    gtk_fixed_put(GTK_FIXED(fix), text_9, 10, 340);

    GtkWidget *loan_duration_text = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(loan_duration_text), g_strdup_printf("<span font='10' foreground='#000000'>%d Month</span>", loan_application.loan_duration));
    gtk_fixed_put(GTK_FIXED(fix), loan_duration_text, 115, 340);



    GtkWidget *generate_score_button = gtk_button_new_with_label("Generate Score");
    gtk_fixed_put(GTK_FIXED(fix), generate_score_button, 30, 445);
    gtk_widget_set_size_request(generate_score_button, 300, 20);


    GtkWidget *Approve_button = gtk_button_new_with_label("Approve");
    gtk_fixed_put(GTK_FIXED(fix), Approve_button, 25, 490);
    gtk_widget_set_size_request(Approve_button, 140, 20);
    gtk_widget_set_visible(Approve_button, FALSE);

    GtkWidget *Reject_button = gtk_button_new_with_label("Reject");
    gtk_fixed_put(GTK_FIXED(fix), Reject_button, 185, 490);
    gtk_widget_set_size_request(Reject_button, 140, 20);
    gtk_widget_set_visible(Reject_button, FALSE);

    g_object_set_data(G_OBJECT(window), "fix", fix);
    g_object_set_data(G_OBJECT(window), "generate_score_button", generate_score_button);
    g_object_set_data(G_OBJECT(window), "Approve_button", Approve_button);
    g_object_set_data(G_OBJECT(window), "Reject_button", Reject_button);

    g_signal_connect(generate_score_button, "clicked", G_CALLBACK(final_step), window);


    // Add the fixed container to the window
    gtk_window_set_child(GTK_WINDOW(window), fix);

    // Present the main window
    gtk_window_present(GTK_WINDOW(window));
}