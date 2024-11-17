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
        pause();
        exit(27);
    } else {
        int code;
        kill(pid, SIGUSR2);
        wait(&code);

        printf("child exited with code: %d\n", code);
    }

    return 0;
}