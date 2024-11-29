//
// Created by PC on 29-Nov-24.
//

#ifndef LOAN_APPLICATIONS_H
#define LOAN_APPLICATIONS_H
typedef struct loan_applications {
    int loan_application_id;
    int user_id;
    float amount_requested;
    char loan_type[50];
    char application_status[50];
    char application_date[11];
    float interest_rate;
    time_t repayment_start_date;
    int loan_duration;
    float

};
#endif //LOAN_APPLICATIONS_H
