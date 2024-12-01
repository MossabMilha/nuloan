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

/* hadi a batale ila bghayti tjibe les info dial chu user mn fichier ou nta 3ndk gha id dialo
 int ReadLoanFromFile(int loan_id, LOAN_APPLICATIONS *loan_application) {
    char file_path[200];
    snprintf(file_path, sizeof(file_path), "C:\\Users\\rachi\\CLionProjects\\nuloan\\DataBase\\LOAN_APPLICATIONS\\user_%d.bin", loan_application->user_id);

    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return -1; // Erreur d'ouverture du fichier
    }

    LOAN_APPLICATIONS temp_loan;
    while (fread(&temp_loan, sizeof(LOAN_APPLICATIONS), 1, file) == 1) {
        if (temp_loan.loan_id == loan_id) {
            *loan_application = temp_loan;
            fclose(file);
            return 0; // Succès
        }
    }

    fclose(file);
    return -2; // Prêt non trouvé
}*/

int MakePaymentTransaction(LOAN_APPLICATIONS loan_application) {
    const char *payment_method = "virement";
    // Vérifier si le prêt est approuvé
    if (strcmp(loan_application.application_status, "approved") != 0) {
        fprintf(stderr, "Erreur : Le prêt %d n'est pas approuvé.\n", loan_application.loan_id);
        return -3; // Prêt non approuvé
    }

    // Calculer le montant mensuel du prêt
    float monthly_payment = CalculateMonthlyPayment(loan_application);

    // Créer une nouvelle transaction de paiement
    TransactionPayment transaction;
    transaction.loan_id = loan_application.loan_id;
    transaction.transaction_id = rand() % 100000; // Générer un ID de transaction unique
    transaction.amount = monthly_payment; // Montant à payer chaque mois
    strcpy(transaction.transaction_type, "repayment"); // Type de transaction : remboursement
    strcpy(transaction.payment_method, payment_method); // Mode de paiement
    strcpy(transaction.transaction_status, "completed"); // Statut de la transaction : terminée
    transaction.transaction_date = time(NULL); // Date et heure de la transaction
    transaction.created_at = time(NULL); // Date de création de la transaction
    transaction.updated_at = time(NULL); // Date de mise à jour de la transaction

    // Construire le chemin du fichier des transactions
    char file_path[200];
    snprintf(file_path, sizeof(file_path), "C:\\Users\\rachi\\CLionProjects\\nuloan\\DataBase\\LOAN_TRANSACTIONS\\user_%d.bin", loan_application.loan_id);

    // Ouvrir le fichier des transactions en mode "append" pour ajouter la transaction
    FILE *file = fopen(file_path, "ab");
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

    // Afficher un message de succès
    printf("Transaction de %.2f ajoutée avec succès pour le prêt %d.\n", monthly_payment, loan_application.loan_id);
    return 0; // Succès
}
/*void FillTransactionData(TransactionPayment *transaction, LOAN_APPLICATIONS loan_application,
                         const char *transaction_type, float amount, const char *payment_method) {
    // Pré-remplir les champs depuis LOAN_APPLICATIONS
    transaction->loan_id = loan_application.loan_id; // ID du prêt
    transaction->transaction_date = time(NULL);      // Date actuelle

    // Champs spécifiques à la transaction
    strcpy(transaction->transaction_type, transaction_type); // Type de transaction
    transaction->amount = amount;                            // Montant de la transaction
    strcpy(transaction->payment_method, payment_method);     // Méthode de paiement

    // Statut initial de la transaction
    strcpy(transaction->transaction_status, "pending");

    // Dates de création et mise à jour
    transaction->created_at = time(NULL);
    transaction->updated_at = time(NULL);

    // Remplissage du champ transaction_id (ici, un exemple simple)
    transaction->transaction_id = rand() % 100000; // Identifiant aléatoire
}


int addTransaction(TransactionPayment transaction, LOAN_APPLICATIONS loan_application) {
    // Vérification de l'existence du prêt
    int loan_status = LoanExistsWithStatus(loan_application);
    if (loan_status == -1) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier des prêts.\n");
        return -1; // Erreur de fichier
    } else if (loan_status == 0) {
        fprintf(stderr, "Erreur : Le prêt %d n'existe pas.\n", loan_application.loan_id);
        return -2; // Prêt non trouvé
    }

    // Validation du montant
    if (transaction.amount <= 0) {
        fprintf(stderr, "Erreur : Le montant de la transaction doit être positif.\n");
        return -3; // Montant invalide
    }

    // Compléter les champs obligatoires de la transaction
    transaction.loan_id = loan_application.loan_id;
    transaction.transaction_date = time(NULL); // Date et heure actuelles
    strcpy(transaction.transaction_status, "pending"); // Statut initial
    transaction.created_at = time(NULL);
    transaction.updated_at = time(NULL);

    // Construction du chemin du fichier des transactions
    char file_path[200];
    snprintf(file_path, sizeof(file_path), "C:\\Users\\rachi\\CLionProjects\\nuloan\\DataBase\\TRANSACTIONS_PAYMENTS\\loan_%d.bin", transaction.loan_id);

    // Ouverture du fichier des transactions en mode append
    FILE *file = fopen(file_path, "ab");
    if (file == NULL) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier des transactions pour le prêt %d.\n", transaction.loan_id);
        return -4; // Erreur lors de l'ouverture du fichier
    }

    // Écriture de la transaction dans le fichier
    if (fwrite(&transaction, sizeof(TransactionPayment), 1, file) != 1) {
        fprintf(stderr, "Erreur : Échec de l'enregistrement de la transaction.\n");
        fclose(file);
        return -5; // Erreur d'écriture
    }

    // Fermeture du fichier
    fclose(file);

    printf("Transaction ajoutée avec succès pour le prêt %d.\n", transaction.loan_id);
    return 0; // Succès
}
/*Code de retour	Description
0	Transaction ajoutée avec succès.
-1	Erreur lors de l'ouverture du fichier des prêts.
-2	Prêt non trouvé.
-3	Montant invalide (non positif).
-4	Erreur lors de l'ouverture du fichier des transactions.
-5	Échec de l'écriture dans le fichier des transactions.*/




