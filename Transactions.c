//
// Created by rachi on 11/30/2024.
//

#include "Transactions.h"



int LoanExistsWithStatus(LOAN_APPLICATIONS loan_application) {
    char file_path[200];
    snprintf(file_path, sizeof(file_path), "C:\\Users\\rachi\\CLionProjects\\nuloan\\DataBase\\LOAN_APPLICATIONS\\user_%d.bin", loan_application.user_id);

    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        return -1; // Fichier introuvable
    }

    LOAN_APPLICATIONS temp_loan;
    while (fread(&temp_loan, sizeof(LOAN_APPLICATIONS), 1, file) == 1) {
        if (temp_loan.loan_id == loan_application.loan_id) {
            fclose(file);
            return 1; // Prêt trouvé
        }
    }

    fclose(file);
    return 0; // Prêt non trouvé
}

float CalculateMonthlyPayment(LOAN_APPLICATIONS loan_application) {
    return loan_application.total_repayment / loan_application.loan_duration;
}
int Loan_Transaction(LOAN_APPLICATIONS loan_application) {
    const char *payment_method = "virement";

    // Vérifier si le prêt est approuvé
    if (strcmp(loan_application.application_status, "approved") != 0) {
        fprintf(stderr, "Erreur : Le prêt %d n'est pas approuvé.\n", loan_application.loan_id);
        return -3; // Prêt non approuvé
    }

    // Calculer le montant mensuel du prêt
    float monthly_payment = CalculateMonthlyPayment(loan_application);

    // Construire le chemin du fichier des transactions
    char file_path[200];
    snprintf(file_path, sizeof(file_path), "C:\\Users\\rachi\\CLionProjects\\nuloan\\DataBase\\LOAN_TRANSACTIONS\\user_%d.bin", loan_application.user_id);

    // Lire le dernier ID de transaction
    int last_transaction_id = 0;
    FILE *file = fopen(file_path, "rb");
    if (file != NULL) {
        TransactionPayment temp_transaction;
        while (fread(&temp_transaction, sizeof(TransactionPayment), 1, file) == 1) {
            last_transaction_id = temp_transaction.transaction_id; // Retenir le dernier ID
        }
        fclose(file);
    }

    // Générer un nouvel ID de transaction
    int new_transaction_id = last_transaction_id + 1;

    // Créer une nouvelle transaction de paiement
    TransactionPayment transaction;
    transaction.loan_id = loan_application.loan_id;
    transaction.transaction_id = new_transaction_id; // Utiliser l'ID généré
    transaction.amount = monthly_payment; // Montant à payer chaque mois
    strcpy(transaction.transaction_type, "repayment"); // Type de transaction : remboursement
    strcpy(transaction.payment_method, payment_method); // Mode de paiement
    strcpy(transaction.transaction_status, "completed"); // Statut de la transaction : terminée
    transaction.transaction_date = time(NULL); // Date et heure de la transaction
    transaction.created_at = time(NULL); // Date de création de la transaction
    transaction.updated_at = time(NULL); // Date de mise à jour de la transaction

    // Ouvrir le fichier des transactions en mode "append" pour ajouter la transaction
    file = fopen(file_path, "ab");
    if (file == NULL) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier des transactions pour le prêt %d.\n", loan_application.loan_id);
        return -4; // Erreur d'ouverture du fichier
    }

    // Écrire la transaction dans le fichier
    if (fwrite(&transaction, sizeof(TransactionPayment), 1, file) != 1) {
        fclose(file);
        fprintf(stderr, "Erreur : Échec de l'enregistrement de la transaction.\n");
        return -5; // Erreur d'écriture
    }

    // Fermer le fichier des transactions
    fclose(file);

    // Mettre à jour le statut du plan de remboursement
    snprintf(file_path, sizeof(file_path), "C:\\Users\\rachi\\CLionProjects\\nuloan\\DataBase\\LOAN_REPAYMENT_SCHEDULES\\user_%d.bin", loan_application.user_id);
    file = fopen(file_path, "rb+");
    if (file == NULL) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier des plans de remboursement pour le prêt %d.\n", loan_application.loan_id);
        return -6; // Erreur d'ouverture du fichier
    }

    LOAN_REPAYMENT_SCHEDULES repayment;
    while (fread(&repayment, sizeof(LOAN_REPAYMENT_SCHEDULES), 1, file) == 1) {
        if (repayment.loan_id == loan_application.loan_id && strcmp(repayment.status, "Pending") == 0) {
            // Mettre à jour le statut et la date de paiement
            strcpy(repayment.status, "Paid");
            time_t now = time(NULL);
            strftime(repayment.payment_date, sizeof(repayment.payment_date), "%Y-%m-%d", localtime(&now));
            fseek(file, -sizeof(LOAN_REPAYMENT_SCHEDULES), SEEK_CUR);
            fwrite(&repayment, sizeof(LOAN_REPAYMENT_SCHEDULES), 1, file);
            break;
        }
    }

    fclose(file);

    // Afficher un message de succès
    printf("Transaction de %.2f ajoutée avec succès pour le prêt %d avec ID de transaction %d.\n",
           monthly_payment, loan_application.loan_id, new_transaction_id);
    return 0; // Succès
}

/*Code de retour	Description
0	Transaction ajoutée avec succès.
-1	Erreur lors de l'ouverture du fichier des prêts.
-2	Prêt non trouvé.
-3	Montant invalide (non positif).
-4	Erreur lors de l'ouverture du fichier des transactions.
-5	Échec de l'écriture dans le fichier des transactions.*/




