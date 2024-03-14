#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "dir_listing.h"

int main(int argc, char* argv[]) {
    const char e_flag[] = "-e";
    const char d_flag[] = "-d";

    if (argc != 3) {
        printf("[!] Usage:\nEncryption: %s -e file_name\nDecryption: %s -d file_name\n", argv[0], argv[0]);
        return 1;
    }

    if (strncmp(e_flag, argv[1], strlen(argv[1])) == 0) {
        printf("Encrypt\n");

        if(!file_exists(argv[2])) {
            printf("[-] File does not exist!\n");
            return 1;
        }

        // DO THE MAGIC HERE

    } else if (strncmp(d_flag, argv[1], strlen(argv[1])) == 0) {
        printf("Decrypt\n");

        if(!file_exists(argv[2])) {
            printf("[-] File does not exist!\n");
            return 1;
        }

        // DO THE MAGIC HERE
    } else {
        printf("[-] Invalid flag\nTry one of these: -e, -d.\n");
        return 1;
    }

    return 0;
}