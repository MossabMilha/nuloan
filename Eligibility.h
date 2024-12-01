//
// Created by rachi on 12/1/2024.
//

#ifndef ELIGIBILITY_H
#define ELIGIBILITY_H
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

// Déclaration des fonctions
void get_current_date(char *buffer, size_t size);

int calculate_credit_score(User user, LOAN_APPLICATIONS loan_application);

Eligibility generate_eligibility(User user, LOAN_TERMS loan_terms, LOAN_APPLICATIONS loan_application);

void save_eligibility_to_file(const Eligibility *eligibility);
#endif //ELIGIBILITY_H
