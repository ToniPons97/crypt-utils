#include <stdbool.h>

#ifndef CRYPTOGRAPHY_H
#define CRYPTOGRAPHY_H

enum decryption_status {
    SUCCESS = 0,
    BAD_SESSION_KEY = 1,
    CANCELLED_BY_USER = 2
};

void encrypt(char* file_name);
void decrypt(char* file_name);
enum decryption_status check_decryption_status(FILE* pipe);
void decryption_status_message(int decryption_status);

#endif
