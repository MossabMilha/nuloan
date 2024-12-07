//
// Created by rachi on 12/2/2024.
//

#ifndef LOAN_TERMS_H
#define LOAN_TERMS_H


// Définition de la structure LOAN_TERMS
typedef struct {
    int loan_term_id;       // ID unique du terme de prêt
    char loan_type[50];     // Type de prêt
    float min_amount;       // Montant minimum
    float max_amount;       // Montant maximum
    int min_duration;       // Durée minimum en mois
    int max_duration;       // Durée maximum en mois
    float interest_rate;    // Taux d'intérêt
    time_t created_at;      // Date de création
} LOAN_TERMS;

#endif //LOAN_TERMS_H
