//
// Created by PC on 29-Nov-24.
//

#ifndef LOAN_APPLICATIONS_H
#define LOAN_APPLICATIONS_H
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "Structures/User.h"
#include "Structures/LOAN_TERMS.h"
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
void get_current_date(char *buffer, size_t size);
void generate_eligibility(User user, LOAN_TERMS loan_terms, LOAN_APPLICATIONS loan_application);
void apply_for_loan(User user);
void display_loan_types();
LOAN_TERMS get_loan_terms_from_file(char loan_type[50]);
#endif //LOAN_APPLICATIONS_H
