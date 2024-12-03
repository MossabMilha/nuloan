//
// Created by rachi on 12/1/2024.
//

#ifndef ELIGIBILITY_H
#define ELIGIBILITY_H
#include <stdio.h>
#include <string.h>
#include <time.h>
#include"Structures/LOAN_APPLICATIONS.h"

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


// Déclaration des fonctions
void get_current_date(char *buffer, size_t size);

int calculate_credit_score(User user, LOAN_APPLICATIONS loan_application);

void generate_eligibility(User user, LOAN_TERMS loan_terms, LOAN_APPLICATIONS loan_application);


#endif //ELIGIBILITY_H
