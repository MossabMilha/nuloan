//
// Created by PC on 29-Nov-24.
//

#ifndef LOAN_TRANSACTIONS_H
#define LOAN_TRANSACTIONS_H
#include <time.h>
typedef struct loan_transactions {
    int transaction_id;
    int loan_id;
    time_t transaction_date;
    char transaction_type[50];
    float amount;
    char payement_method[50];
    char transaction_status[50];
    time_t created_at;
    time_t updated_at;
}loan_transactions;
#endif //LOAN_TRANSACTIONS_H
