#include <stdbool.h>

#ifndef CRYPTOGRAPHY_H
#define CRYPTOGRAPHY_H

enum status {
    SUCCESS = 0,
    BAD_SESSION_KEY = 1,
    CANCELLED_BY_USER = 2
};

void encrypt(char* file_name);
void decrypt(char* file_name);
enum status check_status(FILE* pipe);
void status_message(int status);

#endif
