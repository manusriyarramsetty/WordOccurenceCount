#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

//HELPER FUNCTIONS HEREEE

//check if argument passed is a file or a directory
void check_path(char *path) {
    
    struct stat path_stat;

    // stat() returns 0 on success. If it fails, the path likely doesn't exist.
    if (stat(path, &path_stat) != 0) {
        printf("Path '%s' does not exist or cannot be accessed.\n", path);
        return;
    }

    // Determine what type of path it is
    if (S_ISREG(path_stat.st_mode)) {
        printf("%s is a regular file\n", path);
    } else if (S_ISDIR(path_stat.st_mode)) {
        printf("%s is a directory\n", path);
    } else {
        printf("%s is not a regular file or directory\n", path);
    }
}

//first, parse through the command line argument

int main(int argc, char *argv[]) {

    //default suffix is .txt
    char *suffix = ".txt";

    //index for parsing the argument as an array: "words" term is 0, start at 1
    int i = 1;

    // Parse the optional suffix argument.
    while (i < argc) {

        if (strcmp(argv[i], "-s") == 0) {

            //handles with a space: -s .md
            if (i + 1 >= argc) {
                fprintf(stderr, "Error: missing suffix after -s\n");
                return 1;
            }

            //set suffix
            suffix = argv[i + 1];
            i += 2;

        } else if (strncmp(argv[i], "-s", 2) == 0) {

            //handles without space: -s.md 
            suffix = argv[i] + 2;

            if (*suffix == '\0') {
                fprintf(stderr, "Error: missing suffix after -s\n");
                return 1;
            }

            i++;

        } else {
            break;
        }
    }

    // The remaining arguments are file or directory paths.
    if (i >= argc) {
        fprintf(stderr, "Usage: ./words [-s suffix] file_or_directory ...\n");
        return 1;
    }

    printf("Suffix: %s\n", suffix);

    for (; i < argc; i++) {
        check_path(argv[i]);
    }

    return 0;
}
