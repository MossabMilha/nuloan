//
// Created by rachi on 11/23/2024.
//
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct {
    int eligibility_id;          // ID de l'éligibilité (clé primaire)
    int user_id;                 // ID de l'utilisateur (mn User)
    int credit_score;            // Score de crédit (a hsabe a m3alam hhh)
    float income;                // Revenu de l'utilisateur (mn loan application) khaaeha tzade fi strucure!!!
    float loan_amount_request;   // Montant demandé (de LOAN_APPLICATIONS)
    float approved_amount;       // Montant approuvé (calculé)
    char loan_type[50];          // Type de prêt (de LOAN_TERMS ou LOAN_APPLICATIONS)
    char status[20];             // Statut de la demande de prêt (calculé)
    char created_at[50];         // Date de création
    char updated_at[50];         // Date de mise à jour
} Eligibility;
typedef struct User {
    int user_id;
    char role[10];
    char first_name[50];
    char last_name[50];
    char cin[10];
    char email[50];
    char birthday[11];
    unsigned char password_hashed[50];
    time_t created_at;
    time_t updated_at;
    char status[10];
} User;
typedef struct {
    int loan_id; // Primary Key
    int user_id; // Foreign Key
    float income; // Monthly income
    float amount_requested;
    char loan_type[50];
    char application_status[50];
    char application_date[50];
    float interest_rate;
    char repayment_start_date[50];
    int loan_duration;
    float total_repayment;
    char created_at[50];
    char updated_at[50];
} LOAN_APPLICATIONS;
typedef struct {
    int loan_term_id; // Primary Key
    char loan_type[50];
    float min_amount;
    float max_amount;
    int min_duration; // in months
    int max_duration; // in months
    float interest_rate;
    char created_at[50];
    char updated_at[50];
} LOAN_TERMS;

void get_current_date(char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", t);
}

int calculate_credit_score(User user, LOAN_APPLICATIONS loan_application) {
    int score = 300; // Score initial minimum

    // Critère 1 : Revenus mensuels (pondération forte)
    if (loan_application.income > 10000) {
        score += 200; // Très bons revenus
    } else if (loan_application.income > 5000) {
        score += 100; // Revenus modérés
    } else if (loan_application.income > 3000) {
        score += 50; // Revenus bas mais acceptables
    }

    // Critère 2 : Montant demandé (pondération moyenne)
    if (loan_application.amount_requested < 50000) {
        score += 50; // Montant demandé raisonnable
    } else if (loan_application.amount_requested < 100000) {
        score += 30; // Montant modéré
    } else {
        score -= 50; // Montant élevé, risque accru
    }

    // Critère 3 : Ancienneté du compte utilisateur
    time_t current_time = time(NULL);
    double account_age_years = difftime(current_time, user.created_at) / (365 * 24 * 60 * 60);
    if (account_age_years > 5) {
        score += 100; // Ancienneté élevée
    } else if (account_age_years > 2) {
        score += 50; // Ancienneté modérée
    } else {
        score -= 20; // Compte récent, moins de confiance
    }

    // Critère 4 : Statut utilisateur (bonus si actif)
    if (strcmp(user.status, "active") == 0) {
        score += 50;
    } else {
        score -= 50; // Compte inactif ou douteux
    }

    // Limiter le score entre 300 (minimum) et 850 (maximum)
    if (score > 850) {
        score = 850;
    } else if (score < 300) {
        score = 300;
    }

    return score;
}



Eligibility generate_eligibility(User user, LOAN_TERMS loan_terms, LOAN_APPLICATIONS loan_application) {
    Eligibility eligibility = {0};

    // Remplir les données de base
    eligibility.user_id = user.user_id;


    // Utiliser les données du prêt
    eligibility.credit_score = calculate_credit_score(user, loan_application);
    eligibility.income = loan_application.income;
    eligibility.loan_amount_request = loan_application.amount_requested;
    strncpy(eligibility.loan_type, loan_application.loan_type, sizeof(eligibility.loan_type));

    // Calcul du statut de la demande
    if (eligibility.credit_score >= 650 && eligibility.income >= 3000.0 &&
        eligibility.loan_amount_request >= loan_terms.min_amount &&
        eligibility.loan_amount_request <= loan_terms.max_amount) {
        strncpy(eligibility.status, "Approved", sizeof(eligibility.status));
        eligibility.approved_amount = eligibility.loan_amount_request; // Approuver tout le montant demandé
        } else {
            strncpy(eligibility.status, "Rejected", sizeof(eligibility.status));
            eligibility.approved_amount = 0.0; // Rejet
        }

    // Remplir les champs de date
    get_current_date(eligibility.created_at, sizeof(eligibility.created_at));
    strncpy(eligibility.updated_at, eligibility.created_at, sizeof(eligibility.updated_at));

    return eligibility;
}
void save_eligibility_to_file(const Eligibility *eligibility) {
    // Construire le chemin du fichier avec l'ID utilisateur
    char file_path[200];
    snprintf(file_path, sizeof(file_path), "C:\\Users\\rachi\\CLionProjects\\nuloan\\DataBase\\ELIGIBILITY\\user_%d.bin", eligibility->user_id);
    // Ouvrir le fichier en mode binaire
    FILE *file = fopen(file_path, "wb");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    // Écrire la structure dans le fichier
    if (fwrite(eligibility, sizeof(Eligibility), 1, file) != 1) {
        perror("Erreur lors de l'écriture dans le fichier");
    } else {
        printf("Les données ont été sauvegardées avec succès dans le fichier : %s\n", file_path);
    }

    // Fermer le fichier
    fclose(file);
}
/*int main() { //exemple bach ndire test
    // Créer un utilisateur factice avec des paramètres permettant une éligibilité approuvée
    User user = {0};
    user.user_id = 2;
    user.created_at = time(NULL) - (6 * 365 * 24 * 60 * 60); // Compte créé il y a 6 ans
    strncpy(user.status, "active", sizeof(user.status));

    // Créer une demande de prêt factice qui sera acceptée
    LOAN_APPLICATIONS loan_application = {0};
    loan_application.user_id = user.user_id;
    loan_application.income = 12000.0; // Revenu élevé
    loan_application.amount_requested = 20000.0; // Montant raisonnable
    strncpy(loan_application.loan_type, "Personal Loan", sizeof(loan_application.loan_type));
    strncpy(loan_application.application_status, "Pending", sizeof(loan_application.application_status));
    strncpy(loan_application.application_date, "2024-11-23", sizeof(loan_application.application_date));

    // Créer des termes de prêt factices compatibles avec la demande
    LOAN_TERMS loan_terms = {0};
    strncpy(loan_terms.loan_type, "Personal Loan", sizeof(loan_terms.loan_type));
    loan_terms.min_amount = 10000.0;
    loan_terms.max_amount = 50000.0;
    loan_terms.min_duration = 6;
    loan_terms.max_duration = 60;
    loan_terms.interest_rate = 0.05; // Taux d'intérêt raisonnable

    // Générer l'éligibilité
    Eligibility eligibility = generate_eligibility(user, loan_terms, loan_application);

    // Afficher les détails de l'éligibilité
    printf("ID de l'éligibilité : %d\n", eligibility.eligibility_id);
    printf("ID de l'utilisateur : %d\n", eligibility.user_id);
    printf("Score de crédit : %d\n", eligibility.credit_score);
    printf("Revenu mensuel : %.2f\n", eligibility.income);
    printf("Montant demandé : %.2f\n", eligibility.loan_amount_request);
    printf("Montant approuvé : %.2f\n", eligibility.approved_amount);
    printf("Type de prêt : %s\n", eligibility.loan_type);
    printf("Statut : %s\n", eligibility.status);
    printf("Date de création : %s\n", eligibility.created_at);
    printf("Date de mise à jour : %s\n", eligibility.updated_at);

    // Sauvegarder l'éligibilité dans un fichier
    save_eligibility_to_file(&eligibility);

    return 0;
}*/

    