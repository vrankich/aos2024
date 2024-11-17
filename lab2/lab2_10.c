#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

extern char **environ;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s command [args...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    int fork_res = fork();

    if (fork_res != 0) {
        printf("Parent process parameters:\n");
        for (int i = 0; i < argc; i++) {
             printf("%s\n", argv[i]);
        }

        printf("Environment:\n");
        for (char **env = environ; *env != 0; env++) {
            printf("%s\n", *env);
        }  
    } else {
        int res = execvp(argv[1], argv + 1);
        if (res == -1) {
           perror("unable to execute command");
        }
    }

    return EXIT_SUCCESS;
}