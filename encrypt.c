#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
    const char* filename = NULL;
    filename = argv[1];

    if (argc == 2) {
        char tar_compress_cmd[5000] = "";
        char compressed_name[500] = "";
        char rm_file_cmd[500] = "";
        char gpg_cmd[2000] = "";
        char gpg_output_name[500] = "";

        strncpy(rm_file_cmd, "rm -rf ", sizeof("rm -rf ") + 1);
        strncat(rm_file_cmd, filename, sizeof(filename) + 1);
        strncpy(compressed_name, filename, strlen(filename) + 1);
        strncat(compressed_name, ".tar.gz", strlen(".tar.gz") + 1);
        snprintf(tar_compress_cmd, 5000, "%s%s%s%s", "tar czf ", compressed_name, " ", filename);

        int status = system(tar_compress_cmd);

        if (status == -1) {
            printf("Error executing command: %s\n", tar_compress_cmd);
            return 1;
        }
        
        system(rm_file_cmd);

        strncpy(gpg_output_name, compressed_name, strlen(compressed_name) + 1);
        strncat(gpg_output_name, ".gpg", strlen(".gpg") + 1);
        snprintf(gpg_cmd, 2000, "%s%s%s%s", "gpg --output ", gpg_output_name, " --symmetric --no-symkey-cache ", compressed_name);

        status = system(gpg_cmd);

        if (status == -1) {
            printf("Error executing command: %s\n", gpg_cmd);
            return 1;
        }

        char option;

        printf("Do you wish to delete the unencrypted data? (y/n) ");
        scanf("%c", &option);

        if (option == 'y')
        {
            strncat(rm_file_cmd, ".tar.gz", strlen(".tar.gz") + 1);
            system(rm_file_cmd);
        }
    } else {
        printf("[!] no data to encrypt.\n");
    }

    return 0;
}