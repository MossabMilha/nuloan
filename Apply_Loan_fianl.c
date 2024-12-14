#include "Apply_Loan_fianl.h"

// Array to store loan details
loan_type loans[6];

int generate_unique_id() {
    int temp_id, existing_id;
    int is_unique;
    FILE *id_file;

    // Seed the random number generator
    srand((unsigned int)time(NULL));

    do {
        is_unique = 1; // Assume the ID is unique initially
        temp_id = rand() % 900000 + 100000; // Generate a 6-digit random ID

        // Open the file in read mode to check if the ID already exists
        id_file = fopen("..\\DataBase\\Id_Of_applications.txt", "r");
        if (id_file == NULL) {
            perror("Failed to open ID file for reading");
            return -1; // Return -1 to indicate an error
        }

        // Check each ID in the file against the generated ID
        while (fscanf(id_file, "%d", &existing_id) != EOF) {
            if (temp_id == existing_id) {
                is_unique = 0; // The ID is not unique
                break;
            }
        }

        fclose(id_file);
    } while (!is_unique); // Repeat if the ID is not unique

    // Open the file in append mode to save the unique ID
    id_file = fopen("..\\DataBase\\Id_Of_applications.txt", "a");
    if (id_file == NULL) {
        perror("Failed to open ID file for appending");
        return -1; // Return -1 to indicate an error
    }

    fprintf(id_file, "%d\n", temp_id);
    fclose(id_file);

    return temp_id; // Return the unique ID
}

// Save loan application information
void save_information(GtkWidget *widget, gpointer user_data) {
    GtkWidget *loan_window = GTK_WIDGET(user_data);

    // Retrieve data from entry widgets
    GtkWidget *amount_entry = g_object_get_data(G_OBJECT(loan_window), "amount_entry");
    GtkWidget *duration_entry = g_object_get_data(G_OBJECT(loan_window), "duration_entry");
    GtkWidget *income_entry = g_object_get_data(G_OBJECT(loan_window), "income_entry");
    gint user_id = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(loan_window), "user_id"));
    gint loan_id = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(loan_window), "loan_id"));

    if (!GTK_IS_ENTRY(amount_entry) || !GTK_IS_ENTRY(duration_entry) || !GTK_IS_ENTRY(income_entry)) {
        g_critical("One or more entry widgets are not valid GtkEntry widgets.");
        return;
    }

    GtkEntryBuffer *amount_entry_buffer = gtk_entry_get_buffer(GTK_ENTRY(amount_entry));
    const gchar *amount_text = gtk_entry_buffer_get_text(amount_entry_buffer);

    GtkEntryBuffer *duration_entry_buffer = gtk_entry_get_buffer(GTK_ENTRY(duration_entry));
    const gchar *duration_text = gtk_entry_buffer_get_text(duration_entry_buffer);

    GtkEntryBuffer *income_entry_buffer = gtk_entry_get_buffer(GTK_ENTRY(income_entry));
    const gchar *income_text = gtk_entry_buffer_get_text(income_entry_buffer);

    // Convert text input to appropriate types
    float amount_needed = strtof(amount_text, NULL);
    int duration_needed = atoi(duration_text);
    float income = strtof(income_text, NULL);
    Application app;
    app.loan_application_id = generate_unique_id();
    app.user_id = user_id;
    app.loan_id = loan_id+1;
    app.amount_requested = amount_needed;
    strcpy(app.application_status, "In Progress");
    time_t now = time(NULL);
    strftime(app.application_date, sizeof(app.application_date), "%d-%m-%Y", localtime(&now));
    app.interest_rate = 0.0;
    app.repayment_start_date = 0;
    app.loan_duration = duration_needed;
    app.total_repayment = 0.0;
    app.created_at = now;
    app.updated_at = now;

    char filename_admin[256];
    snprintf(filename_admin, sizeof(filename_admin), "..\\DataBase\\Application_Not_Checked\\application_%d.bin",app.loan_application_id);
    FILE *file_admin = fopen(filename_admin, "wb");
    if (file_admin == NULL) {
        perror("Unable to open file");
        return;
    }
    fwrite(&app, sizeof(app), 1, file_admin);
    fclose(file_admin);

    char filename_user[256];
    snprintf(filename_user, sizeof(filename_user), "..\\DataBase\\Applications\\user_%d.bin",app.user_id);
    FILE *file_member = fopen(filename_user, "ab");
    if (file_member == NULL) {
        perror("Unable to open file");
        return;
    }
    fwrite(&app, sizeof(app), 1, file_member);
    fclose(file_member); // Close the file
}

void Final_Apply(GtkWidget *widget, gpointer user_data) {
    GtkWidget *main_window = GTK_WIDGET(user_data);
    gint user_id = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(main_window), "user_id"));
    gint loan_id = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(main_window), "loan_id"));


    GtkWidget *loan_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(loan_window), "Apply For Loan");
    gtk_window_set_default_size(GTK_WINDOW(loan_window), 350, 600);
    gtk_window_set_resizable(GTK_WINDOW(loan_window), FALSE);

    g_object_set_data(G_OBJECT(loan_window), "user_id", GINT_TO_POINTER(user_id));
    g_object_set_data(G_OBJECT(loan_window), "loan_id", GINT_TO_POINTER(loan_id));

    GtkWidget *loan_fixed = gtk_fixed_new();
    gtk_window_set_child(GTK_WINDOW(loan_window), loan_fixed);

    // Amount Requested Section
    GtkWidget *amount_label = gtk_label_new("Amount Requested:");
    gtk_fixed_put(GTK_FIXED(loan_fixed), amount_label, 25, 75);
    GtkWidget *amount_entry = gtk_entry_new();
    gtk_widget_set_size_request(amount_entry, 300, 30);
    gtk_fixed_put(GTK_FIXED(loan_fixed), amount_entry, 25, 100);

    // Loan Duration Section
    GtkWidget *duration_label = gtk_label_new("Loan Duration (months):");
    gtk_fixed_put(GTK_FIXED(loan_fixed), duration_label, 25, 150);
    GtkWidget *duration_entry = gtk_entry_new();
    gtk_widget_set_size_request(duration_entry, 300, 30);
    gtk_fixed_put(GTK_FIXED(loan_fixed), duration_entry, 25, 175);

    // Monthly Income Section
    GtkWidget *income_label = gtk_label_new("Monthly Income:");
    gtk_fixed_put(GTK_FIXED(loan_fixed), income_label, 25, 225);
    GtkWidget *income_entry = gtk_entry_new();
    gtk_widget_set_size_request(income_entry, 300, 30);
    gtk_fixed_put(GTK_FIXED(loan_fixed), income_entry, 25, 250);

    // Apply Button
    GtkWidget *apply_button = gtk_button_new_with_label("Apply");
    gtk_widget_set_size_request(apply_button, 300, 30);
    gtk_fixed_put(GTK_FIXED(loan_fixed), apply_button, 25, 300);
    g_object_set_data(G_OBJECT(loan_window), "amount_entry", amount_entry);
    g_object_set_data(G_OBJECT(loan_window), "duration_entry", duration_entry);
    g_object_set_data(G_OBJECT(loan_window), "income_entry", income_entry);
    g_signal_connect(apply_button, "clicked", G_CALLBACK(save_information), loan_window);

    // Go Back Button
    GtkWidget *back_button = gtk_button_new_with_label("Go Back");
    gtk_widget_set_size_request(back_button, 300, 30);
    gtk_fixed_put(GTK_FIXED(loan_fixed), back_button, 25, 350);

    // Show the loan application window and hide the current window
    gtk_widget_set_visible(loan_window, TRUE);
    gtk_widget_set_visible(widget, FALSE);
}
