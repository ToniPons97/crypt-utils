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
        char rm_cmd_prefix[] = "rm -rf ";
        char compressed_ext[] = ".tar.gz";
        char encrypted_ext[] = ".gpg";
        char gpg_cmd[2000] = "";
        char gpg_output_name[500] = "";


        strncpy(rm_file_cmd, rm_cmd_prefix, strlen(rm_cmd_prefix));
        strncat(rm_file_cmd, filename, strlen(filename));
        strncpy(compressed_name, filename, strlen(filename));
        strncat(compressed_name, compressed_ext, strlen(compressed_ext));
        snprintf(tar_compress_cmd, 5000, "%s%s%s%s", "tar czf ", compressed_name, " ", filename);

        int status = system(tar_compress_cmd);

        if (status == -1) {
            printf("Error executing command: %s\n", tar_compress_cmd);
            return 1;
        }
        
        system(rm_file_cmd);

        strncpy(gpg_output_name, compressed_name, strlen(compressed_name));
        strncat(gpg_output_name, encrypted_ext, strlen(encrypted_ext));
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
            strncat(rm_file_cmd, compressed_ext, strlen(compressed_ext));
            system(rm_file_cmd);
        }
    } else {
        printf("[!] no data to encrypt.\n");
    }

    return 0;
}