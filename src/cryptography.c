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

    snprintf(rm_file_cmd, 500, "rm -rf %s", file_name);
    snprintf(compressed_name, 500, "%s%s", file_name, compressed_ext);
    snprintf(tar_compress_cmd, 5000, "tar czf %s %s", compressed_name, file_name);

    int status = system(tar_compress_cmd);
    if (status == -1) {
        printf("Error executing command: %s\n", tar_compress_cmd);
        return;
    }

    snprintf(gpg_output_name, 600, "%s%s", compressed_name, encrypted_ext);
    snprintf(gpg_cmd, 6000, "gpg --output %s --symmetric --no-symkey-cache %s 2>&1", gpg_output_name, compressed_name);

    FILE* pipe = popen(gpg_cmd, "r");
    if (pipe == NULL) {
        perror("Error opening pipe.");
        return;
    }

    int encryption_status = check_status(pipe);

    pclose(pipe);

    if (encryption_status == 0) {
        printf("Do you wish to delete the unencrypted data? (y/n) ");
        scanf("%c", &option);

        if (option == 'y') {
            system(rm_file_cmd);
            strncat(rm_file_cmd, compressed_ext, strlen(compressed_ext));
            system(rm_file_cmd);
        } else if (option == 'n') {
            strncat(decompress_cmd, compressed_name, strlen(compressed_name));
            system(decompress_cmd);
        }
    } else {
        strncat(rm_file_cmd, compressed_ext, strlen(compressed_ext));
        system(rm_file_cmd);
        status_message(encryption_status);
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
    int status;
    char option;

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
    snprintf(decrypt_cmd, 2000, "gpg --output %s --decrypt %s 2>&1", decrypted_name, filename_copy);

    FILE* pipe = popen(decrypt_cmd, "r");
    if (pipe == NULL) {
        perror("Error opening pipe.");
        return;
    } 

    int decryption_status = check_status(pipe);

    pclose(pipe);

    if (decryption_status == 0) {
        printf("Decrypted name %s\nOriginal name: %s\n", decrypted_name, file_name);
        snprintf(tar_decompress_cmd, 5000, "tar xzf %s", decrypted_name);

        status = system(tar_decompress_cmd);
        if (status == -1) {
            printf("Error running command %s\n", tar_decompress_cmd);
            return;
        }

        snprintf(rm_cmd,  5000, "rm -rf %s", decrypted_name);        
        status = system(rm_cmd);
        if (status == -1) {
            printf("Error running command %s\n", rm_cmd);
            return;
        }

        printf("Do you wish to delete encrypted file? (y / n): ");
        scanf("%c", &option);

        if (option == 'y') {
            snprintf(rm_cmd, 5000, "rm %s", filename_copy);
            status = system(rm_cmd);
            if (status == -1) {
                printf("Error running command %s\n", rm_cmd);
                return;
            }
        } else {
            printf("Keeping encrypted file.\n");
        }
    } else {
        status_message(decryption_status);
    }
}

enum status check_status(FILE* pipe) {
    char buffer[1024];
    int status = SUCCESS;
    char bad_key_error[] = "gpg: decryption failed: Bad session key";
    char cancelled_by_user_error[] = "gpg: cancelled by user";

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {        
        if (strncmp(buffer, bad_key_error, strlen(bad_key_error)) == 0) {
            status = BAD_SESSION_KEY;
        }
        
        if (strncmp(buffer, cancelled_by_user_error, strlen(cancelled_by_user_error)) == 0) {
            status = CANCELLED_BY_USER;
        }
    }

    return status;
}

void status_message(int status) {
    switch (status)
    {
    case BAD_SESSION_KEY:
        printf("Incorrect passphrase\n");
        break;
    case CANCELLED_BY_USER:
        printf("Operation cancelled by user\n");
        break;
    default:
        printf("Unknown error\n");
        break;
    }
}