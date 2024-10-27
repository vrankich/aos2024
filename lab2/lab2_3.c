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
        printf("child process: PID = %d, PPID = %d, PGID = %d\n", getpid(), getppid(), getpgrp());
        return EXIT_SUCCESS; // завершение дочернего прцесса
    } else {
        printf("parent process: PID = %d, child PID = %d, PGID = %d\n", getpid(), pid, getpgrp());

        int status = 0;
        // ожидание завершения дочернего процесса
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("child process terminated with status code: %d\n", WEXITSTATUS(status));
        } else {
            printf("child process failed\n");
        }
    }

    return 0;
}