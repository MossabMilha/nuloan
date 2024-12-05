//
// Created by rachi on 12/2/2024.
//
#include "LOAN_APPLICATIONS.h"

void display_loan_types() {
    printf("Available Loan Types:\n");
    printf("1. Personal Loan\n");
    printf("2. Home Loan\n");
    printf("3. Car Loan\n");
    printf("4. Student Loan\n");
    printf("5. Business Loan\n");
    printf("6. Agricultural Loan\n");
    printf("7. Renovation Loan\n");
    printf("8. Vacation Loan\n");
    printf("9. Wedding Loan\n");
    printf("10. Microloan\n");
    printf("11. Green Loan\n");
    printf("12. Medical Loan\n");
}
LOAN_TERMS get_loan_terms_from_file(char loan_type[50]) {
    const char *file_path = "C:\\Users\\rachi\\CLionProjects\\nuloan\\DataBase\\LOAN_TERMS\\All_Users.bin";
    FILE *file = fopen(file_path, "rb");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    LOAN_TERMS term;
    while (fread(&term, sizeof(LOAN_TERMS), 1, file) == 1) {
        if (strcmp(term.loan_type, loan_type) == 0) {
            fclose(file);
            return term;
        }
    }

    fclose(file);
    printf("Loan type not found.\n");
    exit(EXIT_FAILURE);
}
/* Objectif :
Cette fonction permet à un utilisateur de faire une demande de prêt en fournissant certaines informations, en choisissant un type de prêt et en recevant un calcul détaillé des conditions de remboursement.
Entrées :
Utilisateur (argument) :

Structure User passée en paramètre contenant les informations de l'utilisateur (ex. user_id).
Données saisies par l'utilisateur :

Revenu mensuel (income) : Saisi au clavier, un nombre décimal représentant le revenu mensuel de l'utilisateur.
Montant demandé (amount_requested) : Saisi au clavier, un nombre décimal représentant le montant souhaité pour le prêt.
Type de prêt : Saisi via un numéro correspondant à l'une des options affichées (1 à 12).
Confirmation (y/n) : Lettre saisie pour confirmer ou annuler la demande.

Affichage à l'écran :

Liste des types de prêts disponibles.
Détails du prêt calculés, notamment :
Type de prêt choisi.
Taux d'intérêt.
Durée du prêt (en mois).
Montant total à rembourser.
Fichiers modifiés :

Enregistrement de la demande dans un fichier binaire spécifique à l'utilisateur si elle est confirmée :
Chemin du fichier :
C:\\Users\\rachi\\CLionProjects\\nuloan\\DataBase\\LOAN_APPLICATIONS\\user_<user_id>.bin
*/
  void apply_for_loan(User user) {
        LOAN_APPLICATIONS loan_application = {0};
        loan_application.user_id = user.user_id;

        printf("Enter your monthly income: ");
        scanf("%f", &loan_application.income);

        printf("Enter the amount requested: ");
        scanf("%f", &loan_application.amount_requested);

        display_loan_types();
        int loan_type_choice;
        printf("Enter the number corresponding to the loan type: ");
        scanf("%d", &loan_type_choice);

        char loan_type[50];
        switch (loan_type_choice) {
            case 1: strcpy(loan_type, "Personal Loan"); break;
            case 2: strcpy(loan_type, "Home Loan"); break;
            case 3: strcpy(loan_type, "Car Loan"); break;
            case 4: strcpy(loan_type, "Student Loan"); break;
            case 5: strcpy(loan_type, "Business Loan"); break;
            case 6: strcpy(loan_type, "Agricultural Loan"); break;
            case 7: strcpy(loan_type, "Renovation Loan"); break;
            case 8: strcpy(loan_type, "Vacation Loan"); break;
            case 9: strcpy(loan_type, "Wedding Loan"); break;
            case 10: strcpy(loan_type, "Microloan"); break;
            case 11: strcpy(loan_type, "Green Loan"); break;
            case 12: strcpy(loan_type, "Medical Loan"); break;
            default:
                printf("Invalid choice.\n");
            return;
        }

        LOAN_TERMS loan_terms = get_loan_terms_from_file(loan_type);
        strcpy(loan_application.loan_type, loan_type);

        // Calculate loan details
        loan_application.interest_rate = loan_terms.interest_rate;
        loan_application.loan_duration = loan_terms.max_duration; // For simplicity, using max duration
        loan_application.total_repayment = loan_application.amount_requested * (1 + loan_application.interest_rate * loan_application.loan_duration / 12);

        // Display loan details
        printf("Loan Details:\n");
        printf("Loan Type: %s\n", loan_application.loan_type);
        printf("Interest Rate: %.2f%%\n", loan_application.interest_rate * 100);
        printf("Loan Duration: %d months\n", loan_application.loan_duration);
        printf("Total Repayment: %.2f\n", loan_application.total_repayment);

        // Confirm application
        char confirm;
        printf("Do you want to confirm the loan application? (y/n): ");
        scanf(" %c", &confirm);

        if (confirm == 'y' || confirm == 'Y') {
            // Generate eligibility
            generate_eligibility(user, loan_terms, loan_application);

            // Save loan application to file
            char file_path[200];
            snprintf(file_path, sizeof(file_path), "C:\\Users\\rachi\\CLionProjects\\nuloan\\DataBase\\LOAN_APPLICATIONS\\user_%d.bin", loan_application.user_id);

            FILE *file = fopen(file_path, "ab");
            if (file == NULL) {
                perror("Error opening file");
                return;
            }

            // Write loan application to file
            if (fwrite(&loan_application, sizeof(LOAN_APPLICATIONS), 1, file) != 1) {
                perror("Error writing to file");
            } else {
                printf("Loan application saved successfully.\n");
            }

            fclose(file);
        } else {
            printf("Loan application cancelled.\n");
        }
    }

