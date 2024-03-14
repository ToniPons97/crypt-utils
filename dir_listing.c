#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>

bool file_exists(char* file_name);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("This program requires 1 argument...\n");
        return 1;
    }

    bool exists = file_exists(argv[1]);

    if (exists)
        printf("EXISTS\n");
    else
        printf("DOESN\'T EXIST\n");
        
    return 0;
}

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