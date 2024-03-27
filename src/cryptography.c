#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cryptography.h"
#include <stdbool.h>

void encrypt(char* file_name) {
    char tar_compress_cmd[5000] = "";
    char compressed_name[500] = "";
    char rm_file_cmd[500] = "";
    char decompress_cmd[] = "tar xzf ";
    char compressed_ext[] = ".tar.gz";
    char encrypted_ext[] = ".gpg";
    char gpg_cmd[6000] = "";
    char gpg_output_name[5000] = "";
    char option;

    snprintf(rm_file_cmd, 500, "%s %s", "rm -rf", file_name);
    snprintf(compressed_name, 500, "%s%s", file_name, compressed_ext);
    snprintf(tar_compress_cmd, 5000, "%s %s %s", "tar czf", compressed_name, file_name);

    int status = system(tar_compress_cmd);
    if (status == -1) {
        printf("Error executing command: %s\n", tar_compress_cmd);
        return;
    }
    
    status = system(rm_file_cmd);
    if (status == -1) {
        printf("Error executing command: %s\n", rm_file_cmd);
        return;
    }

    snprintf(gpg_output_name, 600, "%s%s", compressed_name, encrypted_ext);
    snprintf(gpg_cmd, 6000, "%s %s %s %s", "gpg --output", gpg_output_name, "--symmetric --no-symkey-cache", compressed_name);

    status = system(gpg_cmd);
    if (status == -1) {
        printf("Error executing command: %s\n", gpg_cmd);
        return;
    }

    printf("Do you wish to delete the unencrypted data? (y/n) ");
    scanf("%c", &option);

    if (option == 'y') {
        strncat(rm_file_cmd, compressed_ext, strlen(compressed_ext));
        system(rm_file_cmd);
    } else if (option == 'n') {
        strncat(decompress_cmd, compressed_name, strlen(compressed_name));
        system(decompress_cmd);
    }
}

void decrypt(char* file_name) {
    char filename_copy[600] = "";
    strncpy(filename_copy, file_name, strlen(file_name));

    char decrypted_name[500] = "";
    char delimiter[] = ".";
    char* token = NULL;
    char extensions[30] = "";
    char decrypt_cmd[2000] = "";
    char tar_decompress_cmd[500] = "";
    char rm_cmd[500] = "";
    char buffer[1024];
    char gpg_bad_key_error[] = "gpg: decryption failed: Bad session key";
    bool decryption_success = true;
    int status;

    token = strtok(file_name, delimiter);
    while (token != NULL)
    {
        if (strncmp(token, "gpg", 3) == 0) {
            break;
        }

        if (strncmp(decrypted_name, "", 1) > 0) {
            strncat(decrypted_name, delimiter, strlen(delimiter));
        }

        strncat(decrypted_name, token, strlen(token));
        token = strtok(NULL, delimiter);
    }

    strncat(decrypted_name, extensions, strlen(extensions));
    snprintf(decrypt_cmd, 2000, "%s %s %s %s %s", "gpg --output", decrypted_name, "--decrypt", filename_copy, "2>&1");

    FILE* pipe = popen(decrypt_cmd, "r");
    if (pipe == NULL) {
        perror("Error opening pipe.");
        return;
    } 

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        if (strncmp(buffer, gpg_bad_key_error, strlen(gpg_bad_key_error)) == 0)
            decryption_success = false;
    }

    pclose(pipe);

    if (decryption_success) {
        printf("Decrypted name %s\nOriginal name: %s\n", decrypted_name, file_name);
        snprintf(tar_decompress_cmd, 5000, "%s%s", "tar xzf ", decrypted_name);

        status = system(tar_decompress_cmd);
        if (status == -1) {
            printf("Error running command %s\n", tar_decompress_cmd);
            return;
        }

        snprintf(rm_cmd,  5000, "%s%s", "rm -rf ", decrypted_name);
        status = system(rm_cmd);
        if (status == -1) {
            printf("Error running command %s\n", rm_cmd);
            return;
        }

        snprintf(rm_cmd, 5000, "%s%s", "rm ", filename_copy);
        status = system(rm_cmd);
        if (status == -1) {
            printf("Error running command %s\n", rm_cmd);
            return;
        }
    } else {
        printf("Incorrect passphrase\n");
    }
}