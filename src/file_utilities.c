#include <dirent.h>
#include <string.h>
#include <stdbool.h>
#include "file_utilities.h"
#include <stdio.h>

bool file_exists(char* file_name) {
    DIR *d;

    struct dirent *dir;
    d = opendir(".");

    if (d) {
        while((dir = readdir(d)) != NULL) {
            if (strncmp(file_name, dir->d_name, strlen(file_name)) == 0) {
                closedir(d);
                return true;
            }
        }
        closedir(d);
    }

    return false;
}

bool is_gpg_file(char* file_name) {
    char file_check_cmd[500];
    char buffer[1024];
    char* expected_cmd_out[] = {
        "GPG symmetrically encrypted data (AES256 cipher)",
        "PGP symmetric key encrypted data"
    };

    size_t num_elements = sizeof(expected_cmd_out) / sizeof(expected_cmd_out[0]);

    snprintf(file_check_cmd, sizeof(file_check_cmd), "file %s 2>&1", file_name);

    FILE* pipe = popen(file_check_cmd, "r");
    if (pipe == NULL) {
        perror("Error opening pipe.");
        return false;
    } 

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        for (int i = 0; i < num_elements; i++) {
            if (strstr(buffer, expected_cmd_out[i]) != NULL) {
                pclose(pipe);
                return true;
            }
        }
    }

    pclose(pipe);
    return false;
}