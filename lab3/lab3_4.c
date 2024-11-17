#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>

int main() {
    int pid = fork();
    if (pid == -1) {
        perror("fork");
    }

    if (pid == 0) {
        for (int i = 0; i < 100; i++) {
            printf("iteration [%d]\n", i);
            for (int j = 0; j < 100; j++) {
                for (int k = 0; k < 100; k++) {}
            }
        }
        exit(27);
    } else {
        int code;
        wait(&code);

        if (WIFEXITED(code)) {
            printf("child exited with exit code: %d\n", WEXITSTATUS(code));
        } else {
            printf("child exited with error code\n");
        }
    }

    return 0;
}