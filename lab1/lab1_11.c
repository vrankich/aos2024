#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "program gets 1 or more arguments\n");
        return 1;
    }

    char* filename = NULL;
    size_t max_size = 0;
    struct stat curr_stat = {0};
    for (int i = 1; i < argc; i++) {
        stat(argv[i], &curr_stat);
        size_t curr_size = curr_stat.st_size;
        if (curr_size > max_size) {
            max_size = curr_size;
            filename = argv[i];
        }
    }
    printf("Longest file: %s\nSize: %ld bytes\n", filename, max_size);

    return 0;
}