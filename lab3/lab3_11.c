#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void handler(int sig) {
    printf("child (PID=%d) in handler\n", getpid());
}

int main() {
    struct sigaction sa;
    sa.sa_handler = handler;
    sigaction(SIGUSR1, &sa, NULL);

    for (int i = 0; i < 5; ++i) {
        int pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(1);
        }
        if (pid > 0) {
            printf("process PID=%d created child PID=%d\n", getpid(), pid);
            sleep(2);
        } else {
            sigset_t blockset, emptyset;
            sigemptyset(&blockset);
            sigemptyset(&emptyset);
            sigaddset(&blockset, SIGUSR1);
            sigprocmask(SIG_BLOCK, &blockset, NULL); // block SIGUSR1
            sigsuspend(&emptyset); // wait for signal

            exit(0);
        }
    }

    printf("parent process PID=%d sends SIGUSR1\n", getpid());
    kill(0, SIGUSR1);

    return 0;
}