#include <stdbool.h>

#ifndef CRYPTOGRAPHY_H
#define CRYPTOGRAPHY_H

void encrypt(char* file_name);
void decrypt(char* file_name);
bool check_decryption_status(FILE* pipe);

#endif
