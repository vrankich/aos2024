#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();  // создание нового процесса
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) {
        int ppid = getppid();
        printf("child process: PID = %d, PPID = %d, PGID = %d\n", getpid(), ppid, getpgrp());
        sleep(3);
        printf("child process terminating\n");
    } else {
        printf("parent process: PID = %d, child PID = %d, PGID = %d\n", getpid(), pid, getpgrp());
        printf("parent process terminating\n");
    }

    return 0;
}