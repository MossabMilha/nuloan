//
// Created by PC on 22-Nov-24.
//

#ifndef SEND_EMAIL_H
#define SEND_EMAIL_H
#define ACCESS_TOKEN "ya29.a0ARW5m77JS-DqDDsnvUJBjppwtsHwUCXgKhN5uoI7n6vyPedHqsF-OxMvzH_T0W1VtkVJBMIUdjNw03LhAT9WNW6AApo5a75IbbbjUFn6neWlo2JqZ4EaPpJ7OHORVOUlE108Z7rbnK0VUI--6jZjB0hel1ElHZw01w2maNkpaCgYKAQcSARMSFQHGX2Mizi3TAsGS-ACs9-ag_1q1eg0175"
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
int send_email(const char *to_email, const char *subject, const char *message);

#endif //SEND_EMAIL_H
