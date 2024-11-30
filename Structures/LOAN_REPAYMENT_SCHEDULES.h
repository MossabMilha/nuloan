//
// Created by PC on 29-Nov-24.
//

#ifndef LOAN_REPAYMENT_SCHEDULES_H
#define LOAN_REPAYMENT_SCHEDULES_H
typedef struct loan_repayment_schedules {
    int schedule_id;
    int loan_id;
    float installment_amount;
    time_t due_date;
    char status[50];
    time_t payement_date;
    time_t created_at;
    time_t updated_at;
}loan_repayment_schedules;

#endif //LOAN_REPAYMENT_SCHEDULES_H
