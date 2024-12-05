//
// Created by rachi on 12/1/2024.
//

#include "LOAN_REPAYMENT_SCHEDULES.h"

Eligibility ReadEligibilityFromFile(int loan_id) {
    Eligibility eligibility = {0};
    char file_path[200];
    snprintf(file_path, sizeof(file_path), "C:\\Users\\rachi\\CLionProjects\\nuloan\\DataBase\\ELIGIBILITY\\user_%d.bin", loan_id);

    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        eligibility.eligibility_id = -1; // Erreur d'ouverture du fichier
        return eligibility;
    }

    Eligibility temp_eligibility;
    while (fread(&temp_eligibility, sizeof(Eligibility), 1, file) == 1) {
        if (temp_eligibility.eligibility_id == loan_id) {
            eligibility = temp_eligibility;
            fclose(file);
            return eligibility; // Succès
        }
    }

    fclose(file);
    eligibility.eligibility_id = -2; // Éligibilité non trouvée
    return eligibility;
}

void generate_and_save_repayment_schedule(LOAN_APPLICATIONS loan) {
    Eligibility eligibility = ReadEligibilityFromFile(loan.loan_id);

    if (eligibility.eligibility_id < 0) {
        printf("Erreur lors de la lecture de l'éligibilité (generate_and_save_repayment_schedule) .\n");
        return;
    }

    if (strcmp(eligibility.status, "Approved") != 0) {
        printf("Le prêt n'est pas approuvé.\n");
        return;
    }

    LOAN_REPAYMENT_SCHEDULES repayment;
    float monthly_installment = loan.total_repayment / loan.loan_duration;
    struct tm repayment_date = {0};

    if (sscanf(loan.repayment_start_date, "%d-%d-%d", &repayment_date.tm_year, &repayment_date.tm_mon, &repayment_date.tm_mday) != 3) {
        printf("Erreur : date de début de remboursement invalide.\n");
        return;
    }

    repayment_date.tm_year -= 1900;
    repayment_date.tm_mon -= 1;

    char file_path[256];
    snprintf(file_path, sizeof(file_path),
             "C:\\Users\\rachi\\CLionProjects\\nuloan\\DataBase\\LOAN_REPAYMENT_SCHEDULES\\user_%d.bin", loan.user_id);

    FILE *file = fopen(file_path, "ab");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    printf("Enregistrement du plan de remboursement dans %s\n", file_path);

    for (int i = 0; i < loan.loan_duration; i++) {
        repayment_date.tm_mon += 1;
        mktime(&repayment_date);

        repayment.schedule_id = i + 1;
        repayment.loan_id = loan.loan_id;
        repayment.installment_amount = monthly_installment;
        snprintf(repayment.due_date, sizeof(repayment.due_date),
                 "%d-%02d-%02d", repayment_date.tm_year + 1900, repayment_date.tm_mon + 1, repayment_date.tm_mday);
        strcpy(repayment.status, "Pending");
        strcpy(repayment.payment_date, "");
        snprintf(repayment.created_at, sizeof(repayment.created_at),
                 "%d-%02d-%02d", repayment_date.tm_year + 1900, repayment_date.tm_mon + 1, repayment_date.tm_mday);
        strcpy(repayment.updated_at, repayment.created_at);

        fwrite(&repayment, sizeof(LOAN_REPAYMENT_SCHEDULES), 1, file);
    }

    fclose(file);
    printf("Plan de remboursement enregistré avec succès !\n");
}
void read_repayment_schedule(LOAN_APPLICATIONS loan) {
    LOAN_REPAYMENT_SCHEDULES repayment;
    
    // Construire le chemin du fichier binaire
    char file_path[256];
    snprintf(file_path, sizeof(file_path),
             "C:\\Users\\rachi\\CLionProjects\\nuloan\\DataBase\\LOAN_REPAYMENT_SCHEDULES\\user_%d.bin", loan.user_id);

    // Ouvrir le fichier en mode lecture binaire
    FILE *file = fopen(file_path, "rb");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    // Lire les informations d'échéance du remboursement
    printf("Plan de remboursement pour le prêt ID %d:\n", loan.loan_id);
    while (fread(&repayment, sizeof(LOAN_REPAYMENT_SCHEDULES), 1, file)) {
        printf("Schedule ID: %d\n", repayment.schedule_id);
        printf("Due Date: %s\n", repayment.due_date);
        printf("Installment Amount: %.2f\n", repayment.installment_amount);
        printf("Status: %s\n", repayment.status);
        printf("Payment Date: %s\n", repayment.payment_date);
        printf("Created At: %s\n", repayment.created_at);
        printf("Updated At: %s\n", repayment.updated_at);
        printf("\n");
    }

    fclose(file);
}
/*int main() {
    // Initialiser une demande de prêt
    LOAN_APPLICATIONS loan = {
        .loan_id = 3,
        .user_id = 3  ,
        .income = 50000.0,
        .amount_requested = 114000.0,
        .loan_type = "Personal Loan",
        .application_status = "Approved",
        .application_date = "2023-11-01",
        .interest_rate = 5.0,
        .repayment_start_date = "2023-12-01",
        .loan_duration = 12,
        .total_repayment = 115000.0,
        .created_at = "2023-11-01",
        .updated_at = "2023-11-15"
    };

    // Appeler la fonction pour générer et enregistrer le plan de remboursement
    generate_and_save_repayment_schedule(loan);
    read_repayment_schedule(loan);
    return 0;
}*/