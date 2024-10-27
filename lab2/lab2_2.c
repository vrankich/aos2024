#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    // создание нового процесса
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        // дочерний процесс (сын)
        printf("child process: PID = %d, PPID = %d\n", getpid(), getppid());
    } else {
        // родительский роцесс (отец)
        printf("parent process: PID = %d, child PID = %d\n", getpid(), pid);
    }

    return 0;
}