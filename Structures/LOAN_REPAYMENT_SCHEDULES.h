
//
// Created by rachi on 12/1/2024.
//

#ifndef LOAN_REPAYMENT_SCHEDULES_H
#define LOAN_REPAYMENT_SCHEDULES_H

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


#endif //LOAN_REPAYMENT_SCHEDULES_H
