//
// Created by rachi on 12/2/2024.
//
#include "Structures/LOAN_TERMS.h"
void initialize_loan_terms(LOAN_TERMS *terms, int id, const char *type, float min_amt, float max_amt,
                           int min_dur, int max_dur, float rate) {
    terms->loan_term_id = id;
    strcpy(terms->loan_type, type);
    terms->min_amount = min_amt;
    terms->max_amount = max_amt;
    terms->min_duration = min_dur;
    terms->max_duration = max_dur;
    terms->interest_rate = rate;
    terms->created_at = time(NULL); // Date et heure actuelles
}

// Fonction pour créer et remplir un fichier binaire avec les données de LOAN_TERMS
void create_and_fill_binary_file() {
    const char *file_path = "C:\\Users\\rachi\\CLionProjects\\nuloan\\DataBase\\LOAN_TERMS\\All_Users.bin";

    FILE *file = fopen(file_path, "wb");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    // Tableau de données pour les types de prêts
    LOAN_TERMS terms[] = {
        {1, "Personal Loan", 1000, 50000, 6, 60, 6.5, 0},
        {2, "Home Loan", 50000, 1000000, 12, 360, 3.5, 0},
        {3, "Car Loan", 5000, 100000, 12, 72, 4.0, 0},
        {4, "Student Loan", 5000, 200000, 12, 120, 2.5, 0},
        {5, "Business Loan", 10000, 500000, 6, 120, 5.0, 0},
        {6, "Agricultural Loan", 3000, 200000, 12, 180, 4.5, 0},
        {7, "Renovation Loan", 5000, 100000, 12, 120, 4.8, 0},
        {8, "Vacation Loan", 1000, 20000, 6, 24, 7.0, 0},
        {9, "Wedding Loan", 5000, 50000, 6, 36, 6.0, 0},
        {10, "Microloan", 500, 10000, 3, 36, 8.0, 0},
        {11, "Green Loan", 10000, 300000, 12, 180, 3.0, 0},
        {12, "Medical Loan", 5000, 50000, 6, 60, 5.5, 0}
    };

    // Mise à jour du champ `created_at` pour chaque enregistrement
    for (size_t i = 0; i < sizeof(terms) / sizeof(terms[0]); i++) {
        terms[i].created_at = time(NULL); // Date de création actuelle
        fwrite(&terms[i], sizeof(LOAN_TERMS), 1, file); // Écriture dans le fichier
    }

    fclose(file);
    
}
void read_binary_file() {
    const char *file_path = "C:\\Users\\rachi\\CLionProjects\\nuloan\\DataBase\\LOAN_TERMS\\All_Users.bin";
    
    FILE *file = fopen(file_path, "rb");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    printf("Lecture des données depuis le fichier : %s\n\n", file_path);

    LOAN_TERMS term;
    while (fread(&term, sizeof(LOAN_TERMS), 1, file) == 1) {
        // Affichage des données de chaque terme de prêt
        printf("Loan Term ID: %d\n", term.loan_term_id);
        printf("Loan Type: %s\n", term.loan_type);
        printf("Min Amount: %.2f\n", term.min_amount);
        printf("Max Amount: %.2f\n", term.max_amount);
        printf("Min Duration: %d months\n", term.min_duration);
        printf("Max Duration: %d months\n", term.max_duration);
        printf("Interest Rate: %.2f%%\n", term.interest_rate);
        printf("Created At: %s", ctime(&term.created_at)); // Conversion du timestamp en chaîne
        printf("--------------------------\n");
    }

    fclose(file);
}
/*int main() {
    // Appeler la fonction pour créer et remplir le fichier binaire
    create_and_fill_binary_file();
    read_binary_file();
    return 0;
}*/