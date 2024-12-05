//
// Created by rachi on 12/5/2024.
//

#ifndef LOAN_TERMS_H
#define LOAN_TERMS_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

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
void initialize_loan_terms(LOAN_TERMS *terms, int id, const char *type, float min_amt, float max_amt,
                     int min_dur, int max_dur, float rate);
void create_and_fill_binary_file();
void read_binary_file();
#endif //LOAN_TERMS_H
