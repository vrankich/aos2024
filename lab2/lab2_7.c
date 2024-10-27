#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();  // создание нового процесса
    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        printf("child process: PID = %d, PPID = %d, PGID = %d\n", getpid(), getppid(), getpgrp());
        setpgrp(); //  создает новый процесс группы процессов, который не зависит от родительского
    } else {
        printf("parent process: PID = %d, child PID = %d, PGID = %d\n", getpid(), pid, getpgrp());
    }
    pause();

    return 0;
}