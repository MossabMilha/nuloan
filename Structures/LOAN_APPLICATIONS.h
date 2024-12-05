//
// Created by PC on 29-Nov-24.
//

#ifndef LOAN_APPLICATIONS_H
#define LOAN_APPLICATIONS_H

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

#endif //LOAN_APPLICATIONS_H
