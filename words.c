#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
        printf("Path to process: %s\n", argv[i]);
    }

    return 0;
}
