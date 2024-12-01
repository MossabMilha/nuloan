//
// Created by rachi on 12/1/2024.
//

#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

typedef struct TransactionPayment {
    int transaction_id;           // Identifiant unique pour la transaction (clé primaire)
    int loan_id;                  // Identifiant du prêt (clé étrangère)
    time_t transaction_date;      // Date et heure de la transaction
    char transaction_type[50];    // Type de transaction (paiement, pénalité, remboursement anticipé, etc.)
    float amount;                 // Montant de la transaction (doit être positif)
    char payment_method[50];      // Mode de paiement (carte, virement, espèces, etc.)
    char transaction_status[20];  // Statut de la transaction (complété, en attente, rejeté, etc.)
    time_t created_at;            // Date et heure de création de l'enregistrement
    time_t updated_at;            // Date et heure de la dernière mise à jour
} TransactionPayment;

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
int LoanExistsWithStatus(LOAN_APPLICATIONS loan_application);
int CalculateMonthlyPayment(LOAN_APPLICATIONS loan_application);
int MakePaymentTransaction(LOAN_APPLICATIONS loan_application);

#endif //TRANSACTIONS_H
