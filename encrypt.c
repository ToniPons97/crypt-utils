#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
    const char* filename = NULL;
    filename = argv[1];

    if (argc != 2) {
        printf("[!] no data to encrypt.\n");
        return 1;
    }

    char tar_compress_cmd[5000] = "";
    char compressed_name[500] = "";
    char rm_file_cmd[500] = "";
    char rm_cmd_prefix[] = "rm -rf ";
    char decompress_cmd[] = "tar xzf ";
    char compressed_ext[] = ".tar.gz";
    char encrypted_ext[] = ".gpg";
    char gpg_cmd[2000] = "";
    char gpg_output_name[500] = "";

    snprintf(rm_file_cmd, 500, "%s%s", rm_cmd_prefix, filename);
    snprintf(compressed_name, 500, "%s%s", filename, compressed_ext);
    snprintf(tar_compress_cmd, 5000, "%s%s%s%s", "tar czf ", compressed_name, " ", filename);

    int status = system(tar_compress_cmd);

    if (status == -1) {
        printf("Error executing command: %s\n", tar_compress_cmd);
        return 1;
    }
    
    system(rm_file_cmd);

    snprintf(gpg_output_name, 600, "%s%s", compressed_name, encrypted_ext);
    snprintf(gpg_cmd, 2000, "%s%s%s%s", "gpg --output ", gpg_output_name, " --symmetric --no-symkey-cache ", compressed_name);

    status = system(gpg_cmd);

    if (status == -1) {
        printf("Error executing command: %s\n", gpg_cmd);
        return 1;
    }

    char option;

    printf("Do you wish to delete the unencrypted data? (y/n) ");
    scanf("%c", &option);

    if (option == 'y') {
        strncat(rm_file_cmd, compressed_ext, strlen(compressed_ext));
        system(rm_file_cmd);
    } else if (option == 'n') {
        strncat(decompress_cmd, compressed_name, strlen(compressed_name));
        system(decompress_cmd);
    }

    return 0;
}