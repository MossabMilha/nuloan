//
// Created by rachi on 12/5/2024.
//

#ifndef LOAN_REPAYMENT_SCHEDULES_H
#define LOAN_REPAYMENT_SCHEDULES_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include"LOAN_APPLICATIONS.h"
#include"Eligibility.h"
typedef struct {
    int schedule_id; // Primary Key
    int loan_id; // Foreign Key
    float installment_amount;
    char due_date[50]; // Format YYYY-MM-DD
    char status[50]; // "Pending", "Paid", etc.
    char payment_date[50]; // Date de paiement
    char created_at[50];
    char updated_at[50];
} LOAN_REPAYMENT_SCHEDULES;

void generate_and_save_repayment_schedule(LOAN_APPLICATIONS loan);//génèrer un plan de remboursement pour un prêt donné et enregistre ce plan dans un fichier binaire.
void read_repayment_schedule(LOAN_APPLICATIONS loan);
Eligibility ReadEligibilityFromFile(int loan_id);
#endif //LOAN_REPAYMENT_SCHEDULES_H
