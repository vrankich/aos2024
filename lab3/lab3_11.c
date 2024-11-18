#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <limits.h>

void sync_procs(pid_t pid_input) {
    pid_t pid = getpid();
    sigset_t sigset, empty_sigset;
    sigaddset(&sigset, SIGUSR1);
    sigemptyset(&empty_sigset);
    sigprocmask(SIG_BLOCK, &sigset, NULL);

    if (pid < pid_input) {
        // wait for SIGUSR1
        sigsuspend(&empty_sigset);
    }

    while(1) {
        printf("%d\n", pid);
        sleep(3);
        kill(pid_input, SIGUSR1);
        sigsuspend(&empty_sigset);
    }
}

void handler(int sig) {
    if (sig == SIGUSR1) {
        printf("got SIGUSR1\n");
    } else { 
        SIG_DFL;
    }
}

int main() {
    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, NULL);

    int parent_pid = getpid();
    int child_pid = fork();
    if (child_pid == 0) {
        // child
        sync_procs(parent_pid);
    } else {
        // parent
        synced_procs(child_pid);
    }

    return 0;
}