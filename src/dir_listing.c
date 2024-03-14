#include <dirent.h>
#include <string.h>
#include <stdbool.h>
#include "dir_listing.h"

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