// Created by: MMILHA

#ifndef HASH_PASSWORD_H
#define HASH_PASSWORD_H
#include <string.h>
#include <gcrypt.h>
#include "hash_password.h"
#define key "1A5"
void hash_password(const char *password, unsigned char *output);
#endif //HASH_PASSWORD_H
