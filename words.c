#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

//HELPER FUNCTIONS HEREEE

//rpinting the path using write()
void print_path(const char *path) {
    write(STDOUT_FILENO, path, strlen(path));
    write(STDOUT_FILENO, "\n", 1);
}

//check if given file has the correct suffix
int has_suffix(const char *filename, const char *suffix) {
    size_t name_len = strlen(filename);
    size_t suffix_len = strlen(suffix);

    if (suffix_len > name_len) {
        return 0;
    }

    return strcmp(filename + name_len - suffix_len, suffix) == 0;
}


//need to declare 
void check_path(char *path, char *suffix);
void scan_directory(const char *path, const char *suffix);

//recursive search
void scan_directory(const char *path, const char *suffix) {
    DIR *dir = opendir(path);

    if (dir == NULL) {
        perror(path);
        return;
    }

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {

        /* Ignore hidden files and directories. */
        if (entry->d_name[0] == '.') {
            continue;
        }

        /* Construct the full path to this entry. */
        size_t path_len = strlen(path);
        size_t name_len = strlen(entry->d_name);

        int need_slash = path_len > 0 &&
                         path[path_len - 1] != '/';

        size_t full_len = path_len + need_slash + name_len + 1;
        char *full_path = malloc(full_len);

        if (full_path == NULL) {
            fprintf(stderr, "Error: memory allocation failed\n");
            closedir(dir);
            return;
        }

        snprintf(full_path, full_len, "%s%s%s",
                 path, need_slash ? "/" : "", entry->d_name);

        struct stat info;

        if (stat(full_path, &info) == -1) {
            perror(full_path);

        } else if (S_ISDIR(info.st_mode)) {

            /* Recursively scan the subdirectory. */
            scan_directory(full_path, suffix);

        } else if (S_ISREG(info.st_mode) &&
                   has_suffix(entry->d_name, suffix)) {

            /* Process files that match the suffix. */
            print_path(full_path);
        }

        free(full_path);
    }

    closedir(dir);
}


//check if argument passed is a file or a directory
void check_path(char *path, char *suffix) {
    
    struct stat path_stat;

    // stat() returns 0 on success. If it fails, the path likely doesn't exist.
    if (stat(path, &path_stat) != 0) {
        printf("Path '%s' does not exist or cannot be accessed.\n", path);
        return;
    }

    // Determine what type of path it is
    if (S_ISREG(path_stat.st_mode)) {
        print_path(path);
    } else if (S_ISDIR(path_stat.st_mode)) {
        // Recursively scan this directory
        scan_directory(path, suffix);
    }
}


int main(int argc, char *argv[]) {

    //first, parse through the command line argument
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
        check_path(argv[i], suffix);
    }

    return 0;
}
