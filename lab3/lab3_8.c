#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <limits.h>

int N_ITERATIONS = 1000;

void handler(int s) {
    printf("signal %d recieved\n", s);
    N_ITERATIONS += 100;
}

int main() {
    int pid = fork();
    if (pid == -1) {
        perror("fork");
    }

    if (pid == 0) {
        signal(SIGUSR1, handler);
        for (int i = 0; i < N_ITERATIONS; i++) {
            printf("iteration [%d]\n", i);
            for (int j = 0; j < 1000; j++) {
               for (int k = 0; k < 1000; k++) {}
            }
        }
        exit(27);
    } else {
        sleep(1);

        int code;
        kill(pid, SIGUSR1);
        wait(&code);

        printf("child exited with code: %d\n", code);
    }

    return 0;
}