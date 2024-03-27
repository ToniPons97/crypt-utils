#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "file_utilities.h"
#include "cryptography.h"

int main(int argc, char* argv[]) {
    const char e_flag[] = "-e";
    const char d_flag[] = "-d";

    if (argc != 3) {
        printf("[!] Usage:\nEncryption: %s -e example.txt\nDecryption: %s -d example.txt\n", argv[0], argv[0]);
        return 1;
    }

    if (strncmp(e_flag, argv[1], strlen(argv[1])) == 0) {
        printf("Encrypt\n");

        if(!file_exists(argv[2])) {
            printf("[-] File does not exist!\n");
            return 1;
        }

        encrypt(argv[2]);
    } else if (strncmp(d_flag, argv[1], strlen(argv[1])) == 0) {
        printf("Decrypt\n");

        if(!file_exists(argv[2])) {
            printf("[-] File does not exist!\n");
            return 1;
        }

        if (!is_gpg_file(argv[2])) {
            printf("[!] The file does not appear to be encrypted with GPG using symmetric encryption.\n");
            return 1;
        }

        decrypt(argv[2]);
    } else {
        printf("[-] Invalid flag\nTry one of these: -e, -d.\n");
        return 1;
    }

    return 0;
}