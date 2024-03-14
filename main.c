#include <stdio.h>

int main(int argc, char* argv[]) {
    const char e_flag[] = "-e";
    const char d_flag[] = "-d";

    if (argc != 3) {
        printf("Usage:\nEncryption: %s -e file_name\nDecryption: %s -d file_name\n", argv[0], argv[0]);
        return 1;
    }


}