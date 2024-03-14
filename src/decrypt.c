#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("[!] no file or directoy to encrypt.\n");
        return 1;
    }

    char* filename = NULL;
    filename = argv[1];
    char filename_copy[600] = "";

    strncpy(filename_copy, filename, strlen(filename));

    char decrypted_name[500] = "";
    char delimiter[] = ".";
    char* token = NULL;
    char extensions[30] = "";
    char decrypt_cmd[2000] = "";

    token = strtok(filename, delimiter);
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
    snprintf(decrypt_cmd, 2000, "%s%s%s%s", "gpg --output ", decrypted_name, " --decrypt ", filename_copy);
    
    system(decrypt_cmd);
}