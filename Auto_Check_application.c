//
// Created by PC on 08-Dec-24.
//

#include "Auto_Check_application.h"
int count_files_in_directory_2(const char *directory_path) {
    int file_count = 0;
    struct dirent *entry;
    DIR *dp = opendir(directory_path);

    if (dp == NULL) {
        return 0;
    }

    while ((entry = readdir(dp))) {
        struct stat path_stat;
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", directory_path, entry->d_name);
        stat(full_path, &path_stat);
        if (S_ISREG(path_stat.st_mode)) {
            file_count++;
        }
    }

    closedir(dp);
    return file_count;
}
void update_application_status(const char *file_name, int target_id, const char *new_status) {
    FILE *file = fopen(file_name, "rb+");
    if (!file) {
        perror("Error opening file");
        return;
    }

    Application app;
    while (fread(&app, sizeof(Application), 1, file) == 1) {
        if (app.loan_application_id == target_id) {
            // Update the application status
            strncpy(app.application_status, new_status, sizeof(app.application_status) - 1);
            app.application_status[sizeof(app.application_status) - 1] = '\0';

            // Update the timestamp
            app.updated_at = time(NULL);

            // Move the file pointer back to overwrite the structure
            fseek(file, -(long)sizeof(Application), SEEK_CUR);
            fwrite(&app, sizeof(Application), 1, file);
            fclose(file);
            return;
        }
    }

    fclose(file);
}
void delete_id_from_file(const char *file_name, int id_to_delete) {
    // Open the file in read mode
    FILE *file = fopen(file_name, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    // Read all IDs into memory
    int ids[1000];  // Assuming there will be no more than 1000 IDs
    int count = 0;

    while (fscanf(file, "%d", &ids[count]) == 1) {
        count++;
    }
    fclose(file);

    // Open the file in write mode to overwrite the content
    file = fopen(file_name, "w");
    if (!file) {
        perror("Error opening file for writing");
        return;
    }

    // Write back all IDs except the one to delete
    for (int i = 0; i < count; i++) {
        if (ids[i] != id_to_delete) {
            fprintf(file, "%d\n", ids[i]);
        }
    }

    fclose(file);
}

void Auto_Check_app(){
  printf("Auto_Check_app\n");
    sir_ltema:
        int max = count_files_in_directory_2("..\\DataBase\\Application_Not_Checked");
    for(int i = 1; i <= max; i++){
        FILE *id_file = fopen("..\\DataBase\\Id_Of_applications.txt", "r");
        if (id_file == NULL) {
            perror("Failed to open ID file");
            return; // Return an empty Loan_Apply struct
        }

        int id;
        int current_line = 0;
        while (fscanf(id_file, "%d", &id) != EOF) {
            if (current_line == i - 1) {
                break;
            }
            current_line++;
        }
        fclose(id_file);

        if (current_line != i - 1) {
            fprintf(stderr, "Line number %d not found in ID file\n", i);
            return; // Return an empty Loan_Apply struct
        }

        char bin_file_path[256];

        snprintf(bin_file_path, sizeof(bin_file_path), "..\\DataBase\\Application_Not_Checked\\application_%d.bin", id);
        FILE *bin_file = fopen(bin_file_path, "rb");
        if (bin_file == NULL) {
            perror("Failed to open binary file");
            return; // Return an empty Loan_Apply struct
        }

        Application loan_apply;
        fread(&loan_apply, sizeof(loan_apply), 1, bin_file);
        fclose(bin_file);

        char loan_path[256];
        snprintf(loan_path, sizeof(loan_path), "..\\DataBase\\LOANS\\loan_%d.bin", loan_apply.loan_id);
        FILE *loan_file = fopen(loan_path, "rb");
        if (loan_file == NULL) {
            perror("Failed to open binary file");
            return; // Return an empty Loan_Apply struct
        }
        loan_type loan;
        fread(&loan, sizeof(loan), 1, loan_file);
        fclose(loan_file);
        printf("%f\n",loan_apply.amount_requested);
        printf("%f\n====\n",loan.min_amount);
        if (!((loan_apply.amount_requested >= loan.min_amount && loan_apply.amount_requested <= loan.max_amount) && (loan_apply.loan_duration >= loan.min_duration && loan_apply.loan_duration <= loan.max_duration))) {
            char file_name[256];
            snprintf(file_name, sizeof(file_name), "..\\DataBase\\Applications\\user_%d.bin", loan_apply.user_id);
            update_application_status(file_name, loan_apply.loan_application_id, "Declined");

            delete_id_from_file("..\\DataBase\\Id_Of_applications.txt", loan_apply.loan_application_id);

            snprintf(file_name, sizeof(file_name), "..\\DataBase\\Application_Not_Checked\\application_%d.bin", loan_apply.loan_application_id);
            if (remove(file_name) == 0) {
                goto sir_ltema;
                printf("File '%s' deleted successfully.\n", file_name);
            } else {
                perror("Error deleting file");
            }
        }
    }

};