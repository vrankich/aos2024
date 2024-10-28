#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char * argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s command [args...]\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    pid_t pid = fork();
    if (pid == -1) {
        fprintf(stderr, "usage: %s command [args...]\n", argv[0]);
        return EXIT_FAILURE;    
    }

    if (pid == 0) {
        execvp(argv[1], argv + 1);
    }

    return EXIT_SUCCESS;
}