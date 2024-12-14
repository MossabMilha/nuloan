#include "Approve_Reject_Application.h"
#include "Structures/User.h"
#include "Structures/Application.h"
#include "Structures/Loans_Types.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

User user;
Application loan_application;
loan_type loan;
int loan_application_id;
float chosen_interest_rate;

float suggest_interest_rate(int score, loan_type loan) {
    float interest_rate;
    if (score > 750) {
        interest_rate = loan.interest_rate[0];
    } else if (score > 650) {
        interest_rate = (loan.interest_rate[0] + loan.interest_rate[1]) / 2;
    } else {
        interest_rate = loan.interest_rate[1];
    }
    return interest_rate;
}

int calculate_credit_score(User user, Application loan_application, loan_type loan) {
    int score = 300;
    time_t current_time = time(NULL);

    if (loan_application.loan_duration >= loan.min_duration && loan_application.loan_duration <= loan.max_duration) {
        score += 30;
    } else {
        score -= 30;
    }

    double account_age_years = difftime(current_time, user.created_at) / (365.0 * 24.0 * 60.0 * 60.0);
    if (account_age_years > 5) {
        score += 100;
    } else if (account_age_years > 2) {
        score += 50;
    } else {
        score -= 20;
    }

    if (strcmp(user.status, "active") == 0) {
        score += 50;
    } else {
        score -= 50;
    }

    if (strcmp(loan_application.application_status, "Approved") == 0) {
        score += 30;
    } else if (strcmp(loan_application.application_status, "Declined") == 0) {
        score -= 50;
    } else if (strcmp(loan_application.application_status, "In Progress") == 0) {
        score += 10;
    }

    if (difftime(current_time, loan_application.repayment_start_date) < 0) {
        score += 10;
    } else {
        score -= 20;
    }

    if (score > 850) {
        score = 850;
    } else if (score < 300) {
        score = 300;
    }

    return score;
}

Application Read_application(int id) {
    char bin_file_path[256];
    snprintf(bin_file_path, sizeof(bin_file_path), "..\\DataBase\\Application_Not_Checked\\application_%d.bin", id);
    FILE *bin_file = fopen(bin_file_path, "rb");
    if (bin_file == NULL) {
        perror("Failed to open binary file");
        return (Application){0};
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
        return (User){0};
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
        return (loan_type){0};
    }
    loan_type loan;
    fread(&loan, sizeof(loan), 1, bin_file);
    fclose(bin_file);
    return loan;
}

void Approve_Application(GtkWidget *window) {
    printf("\nHello\n");
    Application updated_application = loan_application;
    strcpy(updated_application.application_status, "Approved");
    updated_application.interest_rate = chosen_interest_rate;

    struct tm *tm_info;
    time_t current_time = time(NULL);
    tm_info = localtime(&current_time);
    if (tm_info->tm_mday > 10) {
        tm_info->tm_mon += 2;
    } else {
        tm_info->tm_mon += 1;
    }
    tm_info->tm_mday = 1;
    updated_application.repayment_start_date = mktime(tm_info);
    updated_application.total_repayment = updated_application.amount_requested * (1 + chosen_interest_rate / 100);
    updated_application.updated_at = current_time;

    char user_bin_file_path[256];
    snprintf(user_bin_file_path, sizeof(user_bin_file_path), "..\\DataBase\\Applications\\user_%d.bin", updated_application.user_id);

    FILE *user_bin_file = fopen(user_bin_file_path, "rb+");
    if (user_bin_file == NULL) {
        perror("Failed to open binary file");
        return;
    }

    Application temp_application;
    while (fread(&temp_application, sizeof(Application), 1, user_bin_file) == 1) {
        if (temp_application.loan_application_id == updated_application.loan_application_id) {
            fseek(user_bin_file, -sizeof(Application), SEEK_CUR);
            fwrite(&updated_application, sizeof(Application), 1, user_bin_file);
            fclose(user_bin_file);
            printf("Loan application approved and updated successfully.\n");
        }
    }

    char application_path[256];
    snprintf(application_path, sizeof(application_path), "..\\DataBase\\Application_Not_Checked\\application_%d.bin", updated_application.loan_application_id);
    printf("%s\n", application_path);
    if (remove(application_path) == 0) {
        printf("Application removed from the not checked folder\n");
    }

    // Now we will remove the ID from the Id_Of_applications.txt
    char id_path[256];
    snprintf(id_path, sizeof(id_path), "..\\DataBase\\Id_Of_applications.txt");

    // Open the Id_Of_applications.txt file for reading
    FILE *id_file = fopen(id_path, "r");
    if (id_file == NULL) {
        perror("Failed to open ID file");
        return;
    }

    // Temporary file to store the new contents without the deleted ID
    FILE *temp_file = fopen("temp.txt", "w");
    if (temp_file == NULL) {
        perror("Failed to open temporary file");
        fclose(id_file);
        return;
    }

    int id;
    while (fscanf(id_file, "%d", &id) == 1) {
        if (id != updated_application.loan_application_id) {
            fprintf(temp_file, "%d\n", id);  // Write ID to temp file if it doesn't match
        }
    }

    fclose(id_file);
    fclose(temp_file);

    // Replace the original file with the temporary file
    if (remove(id_path) != 0) {
        perror("Failed to remove the original ID file");
    } else {
        if (rename("temp.txt", id_path) != 0) {
            perror("Failed to rename temporary file");
        } else {
            printf("ID successfully deleted from the file.\n");
        }
    }

    // Close the user file and finish
    fclose(user_bin_file);
}
void Reject_Application(GtkWidget *window) {
    Application updated_application = loan_application;
    strcpy(updated_application.application_status, "Rejected");
    updated_application.updated_at = time(NULL);

    char user_bin_file_path[256];
    snprintf(user_bin_file_path, sizeof(user_bin_file_path), "..\\DataBase\\Applications\\user_%d.bin", updated_application.user_id);

    FILE *user_bin_file = fopen(user_bin_file_path, "rb+");
    if (user_bin_file == NULL) {
        perror("Failed to open binary file");
        return;
    }

    Application temp_application;
    while (fread(&temp_application, sizeof(Application), 1, user_bin_file) == 1) {
        if (temp_application.loan_application_id == updated_application.loan_application_id) {
            fseek(user_bin_file, -sizeof(Application), SEEK_CUR);
            fwrite(&updated_application, sizeof(Application), 1, user_bin_file);
            fclose(user_bin_file);
            printf("Loan application rejected and updated successfully.\n");
        }
    }

    char application_path[256];
    snprintf(application_path, sizeof(application_path), "..\\DataBase\\Application_Not_Checked\\application_%d.bin", updated_application.loan_application_id);
    printf("%s\n", application_path);
    if (remove(application_path) == 0) {
        printf("Application removed from the not checked folder\n");
    }

    // Now we will remove the ID from the Id_Of_applications.txt
    char id_path[256];
    snprintf(id_path, sizeof(id_path), "..\\DataBase\\Id_Of_applications.txt");

    // Open the Id_Of_applications.txt file for reading
    FILE *id_file = fopen(id_path, "r");
    if (id_file == NULL) {
        perror("Failed to open ID file");
        return;
    }

    // Temporary file to store the new contents without the deleted ID
    FILE *temp_file = fopen("temp.txt", "w");
    if (temp_file == NULL) {
        perror("Failed to open temporary file");
        fclose(id_file);
        return;
    }

    int id;
    while (fscanf(id_file, "%d", &id) == 1) {
        if (id != updated_application.loan_application_id) {
            fprintf(temp_file, "%d\n", id);  // Write ID to temp file if it doesn't match
        }
    }

    fclose(id_file);
    fclose(temp_file);
}


void final_step(GtkWidget *app, gpointer user_data) {
    GtkWidget *window = GTK_WIDGET(user_data);
    GtkWidget *fix = g_object_get_data(G_OBJECT(window), "fix");
    GtkWidget *generate_score_button = g_object_get_data(G_OBJECT(window), "generate_score_button");
    gtk_button_set_label(GTK_BUTTON(generate_score_button), "Approve With my Score");
    GtkWidget *Approve_button = g_object_get_data(G_OBJECT(window), "Approve_button");
    GtkWidget *Reject_button = g_object_get_data(G_OBJECT(window), "Reject_button");

    int score = calculate_credit_score(user, loan_application, loan);
    chosen_interest_rate = suggest_interest_rate(score, loan);

    GtkWidget *text_10 = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(text_10), g_strdup_printf("<span font='10' weight='bold' foreground='red'>Score : %d</span>", score));
    gtk_fixed_put(GTK_FIXED(fix), text_10, 35, 420);

    GtkWidget *text_11 = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(text_11), g_strdup_printf("<span font='10' weight='bold' foreground='red'>Interest Suggest : %.2f%%</span>", chosen_interest_rate));
    gtk_fixed_put(GTK_FIXED(fix), text_11, 150, 420);

    if (GTK_IS_WIDGET(Approve_button)) {
        gtk_widget_set_visible(Approve_button, TRUE);
        g_signal_connect(Approve_button, "clicked", G_CALLBACK(Approve_Application), window);
    }
    if (GTK_IS_WIDGET(Reject_button)) {
        gtk_widget_set_visible(Reject_button, TRUE);
        g_signal_connect(Approve_button, "clicked", G_CALLBACK(Reject_Application), window);
    }
}

void Approve_Activate(GtkApplication *app, gpointer user_data) {
    loan_application_id = GPOINTER_TO_INT(user_data);
    printf("Loan Application ID: %d\n", loan_application_id);

    loan_application = Read_application(loan_application_id);
    user = Read_User_1(loan_application.user_id);
    loan = Read_Loan_1(loan_application.loan_id);

    GtkWidget *window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(window), "Window");
    gtk_window_set_default_size(GTK_WINDOW(window), 350, 600);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    GtkWidget *fix = gtk_fixed_new();

    GtkWidget *title_1 = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title_1), "<span font='15' weight='bold' foreground='#000000'>User Information</span>");
    gtk_fixed_put(GTK_FIXED(fix), title_1, 10, 10);

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
    gtk_fixed_put(GTK_FIXED(fix), Full_Name_text, 90, 80);

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
    gtk_fixed_put(GTK_FIXED(fix), birthday_text, 80, 140);

    GtkWidget *title_2 = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title_2), "<span font='15' weight='bold' foreground='#000000'>Application Information</span>");
    gtk_fixed_put(GTK_FIXED(fix), title_2, 10, 180);

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

    gtk_window_set_child(GTK_WINDOW(window), fix);
    gtk_window_present(GTK_WINDOW(window));
}