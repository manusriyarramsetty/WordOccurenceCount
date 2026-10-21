#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include <ctype.h>

//use ts for open() and O_RDNLY in our countin word method
#include <fcntl.h>

/* hello manu! i wrote helper functions and added comments for what they do and where
theyre gonna be called in the word counting method's logic in my head. i didnt finish 
the word counting method, i js have like the start, where it sets it up and then the two loops
to parse through and process each character. u get to finish that and then handle the
sorting and printing the words by count*/


//HELPER FUNCTIONS HEREEE
struct WordCount {
    char *word;
    size_t count;
    struct WordCount *next;
};

struct WordCount *word_counts = NULL;

static void add_word(const char *word) {
    struct WordCount *current = word_counts;
    while (current != NULL) {
        if (strcmp(current->word, word) == 0) {
            current->count++;
            return;
        }
        current = current->next;
    }
    struct WordCount *item = malloc(sizeof(*item));
    if (item == NULL) {
        perror("malloc");
        exit(1);
    }
    item->word = malloc(strlen(word) + 1);
    if (item->word == NULL) {
        perror("malloc");
        free(item);
        exit(1);
    }
    strcpy(item->word, word);
    item->count = 1;
    item->next = word_counts;
    word_counts = item;
}

static int is_letter(char c) {
    return (c>= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

/* if character is a letter or a valid apostrophe (aka not after a hyphen/is after a letter) 
call this function in our word counting method. also not sure if apostrophes without a letter 
after it counts. does foo' count? i think it does the assignment dont specify */

static void append_char(char **word, size_t *length, size_t *capacity, char c) {
    
    //make more space if full
    if (*length + 1 >= *capacity) {
        size_t new_capacity = *capacity * 2;
        char *temp = realloc(*word, new_capacity);

        if (temp == NULL) {
            perror("realloc");
            free(*word);
            exit(1);
        }

        *word = temp;
        *capacity = new_capacity;
    }

    (*word)[*length] = c;
    (*length)++;
    (*word)[*length] = '\0';
}

/* call this function in our word counting method if we got a invalid hyphen, if we have
an ending character (aka anything thats not a letter, apostrophe, or hyphen), or wtv 
this also uses add word correctly, i hope*/
static void finish_word(char *word, size_t *length) {
    if (*length > 0) {
        add_word(word);
        *length = 0;
        word[0] = '\0';
    }
}



/* this is entirely for testing, so ignore during word counting method implementation
(got ts online) -> shld be included in design notes */

static void print_counts_for_testing(void) {
    struct WordCount *current = word_counts;

    while (current != NULL) {
        printf("%s %zu\n", current->word, current->count);
        current = current->next;
    }
}



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
void count_words(const char *path); 


//recursive search
void scan_directory(const char *path, const char *suffix) {
    DIR *dir = opendir(path);

    if (dir == NULL) {
        perror(path);
        return;
    }

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {

        if (entry->d_name[0] == '.') {
            continue;
        }

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
            //recurse again
            scan_directory(full_path, suffix);

        } else if (S_ISREG(info.st_mode) &&
                   has_suffix(entry->d_name, suffix)) {

            //REPLACE
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
        //REPLACE
    } else if (S_ISDIR(path_stat.st_mode)) {
        // Recursively scan this directory
        scan_directory(path, suffix);
    }
}


/* gonna start writing counting word method here. THIS IS INCOMPLETEE i didnt finish 
i js have the basic logic?*/


void count_words(const char *path) {

    //open file, error if open fails

    int fd = open(path, O_RDONLY);

    if (fd == -1) {
        perror(path);
        return;
    }

    //shld i make ts bigger. temporary capacity of each word, im js sayin its gonna 
    //reach a max of 32 bytes rn
    size_t capacity = 32;
    size_t length = 0;

    char *word = malloc(capacity);
    if (word == NULL) {
        perror("malloc");
        close(fd);
        exit(1);
    }

    //starting word!
    word[0] = '\0';

    //read files in chunks of buffer size 4096 bytes
    char buffer[4096];
    ssize_t bytes_read;

    // A hyphen is held until we know whether a letter follows it, or its invalid
    int isHyphen = 0;

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {

        for (ssize_t i = 0; i < bytes_read; i++) {
            
            char c = buffer[i];

            //four cases, letter, hyphen, apostrophe, or end word character

            
        }
    }

}


int main(int argc, char *argv[]) {

    //first, parse through the command line argument
    //default suffix is .txt
    char *suffix = ".txt";

    //index for parsing the argument as an array: "words" term is 0, start at 1
    int i = 1;

    // Parse the optional suffix argument
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

    // The remaining arguments are file or directory paths
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
