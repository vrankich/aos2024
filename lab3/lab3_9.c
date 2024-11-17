#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <limits.h>

void handler(int s) {
    printf("signal %d recieved\n", s);
}

int main() {
    int pid = fork();
    if (pid == -1) {
        perror("fork");
    }

    if (pid == 0) {
        signal(SIGALRM, handler);
        alarm(2);
        pause();
        exit(1);
    } else {
        int code;
        wait(&code);

        printf("child exited with code: %d\n", code);
    }

    return 0;
}