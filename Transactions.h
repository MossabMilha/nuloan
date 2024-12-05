//
// Created by rachi on 12/5/2024.
//

#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include"LOAN_REPAYMENT_SCHEDULES.h"
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

int LoanExistsWithStatus(LOAN_APPLICATIONS loan_application);
float CalculateMonthlyPayment(LOAN_APPLICATIONS loan_application);
int Loan_Transaction(LOAN_APPLICATIONS loan_application);
#endif //TRANSACTIONS_H
