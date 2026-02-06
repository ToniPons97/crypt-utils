#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cryptography.h"
#include <stdbool.h>

void encrypt(char* file_name) {
    char* dy_tar_compress_cmd = NULL;
    char* compressed_name = NULL;
    char* rm_file_cmd = NULL;
    char* decompress_cmd = "tar xzf ";
    char* compressed_ext = ".tar.gz";
    char* encrypted_ext = ".gpg";
    char* gpg_cmd = NULL;
    char* gpg_output_name = NULL;
    char option;
    
    size_t dy_rm_file_cmd_len = strlen("rm -rf ") + strlen(file_name) + 1;
    rm_file_cmd = (char*)malloc(dy_rm_file_cmd_len);
    if (rm_file_cmd == NULL) {
        printf("Error allocating memory for rm_file_cmd\n");
        goto out;
    }

    snprintf(rm_file_cmd, dy_rm_file_cmd_len, "rm -rf %s", file_name);

    size_t dy_compressed_name_len = strlen(file_name) + strlen(compressed_ext) + 1;
    compressed_name = (char*)malloc(dy_compressed_name_len);
    if (compressed_name == NULL) {
        printf("Error allocating memory for compressed_name\n");
        goto out;
    }

    snprintf(compressed_name, dy_compressed_name_len, "%s%s", file_name, compressed_ext);

    size_t dy_tar_compress_cmd_len = strlen("tar czf ") + strlen(compressed_name) + strlen(file_name) + 2;
    dy_tar_compress_cmd = (char*)malloc(dy_tar_compress_cmd_len);
    if (dy_tar_compress_cmd == NULL) {
        printf("Error allocating memory for dy_tar_compress_cmd\n");
        goto out;
    }

    snprintf(dy_tar_compress_cmd, dy_tar_compress_cmd_len, "tar czf %s %s", compressed_name, file_name);

    int status = system(dy_tar_compress_cmd);
    if (status == -1) {
        printf("Error executing command: %s\n", dy_tar_compress_cmd);
        goto out;
    }

    size_t dy_gpg_output_name_len = strlen(compressed_name) + strlen(encrypted_ext) + 1;
    gpg_output_name = (char*)malloc(dy_gpg_output_name_len);
    if (gpg_output_name == NULL) {
        printf("Error allocating memory for gpg_output_name\n");
        goto out;
    }

    snprintf(gpg_output_name, dy_gpg_output_name_len, "%s%s", compressed_name, encrypted_ext);


    size_t dy_gpg_cmd_len = strlen("gpg --output --symmetric --no-symkey-cache 2>&1") + strlen(gpg_output_name) + strlen(compressed_name) + 5;
    gpg_cmd = (char*)malloc(dy_gpg_cmd_len);
    if (gpg_cmd == NULL) {
        printf("Error allocating memory for gpg_cmd\n");
        goto out;
    }

    snprintf(gpg_cmd, dy_gpg_cmd_len, "gpg --output %s --symmetric --no-symkey-cache %s 2>&1", gpg_output_name, compressed_name);

    FILE* pipe = popen(gpg_cmd, "r");
    if (pipe == NULL) {
        perror("Error opening pipe.");
        goto out;
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
            size_t decompress_cmd_len = strlen(decompress_cmd) + strlen(compressed_name) + 2;
            char* dy_decompress_cmd = (char*)malloc(decompress_cmd_len);
            if (!dy_decompress_cmd) {
                perror("No mem");
                goto out;
            }

            snprintf(dy_decompress_cmd, decompress_cmd_len, "%s %s", decompress_cmd, compressed_name);
            system(dy_decompress_cmd);

            if (dy_decompress_cmd)
                free(dy_decompress_cmd);
        }
    } else {
        strncat(rm_file_cmd, compressed_ext, strlen(compressed_ext));
        system(rm_file_cmd);
        status_message(encryption_status);
    }

out:
    if (dy_tar_compress_cmd)
        free(dy_tar_compress_cmd);

    if (compressed_name)
        free(compressed_name);

    if (gpg_output_name)
        free(gpg_output_name);

    if (rm_file_cmd)
        free(rm_file_cmd);

    if (gpg_cmd)
        free(gpg_cmd);
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
