#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <limits.h>

void handler(int s) {
    if (s == SIGINT) {
        printf("caught SIGINT\n");
    } else {
        signal(s, SIG_DFL);
    }
}

int main() {
    signal(SIGINT, handler);

    sigset_t sigset;
    sigaddset(&sigset, SIGINT);
    sigprocmask(SIG_BLOCK, &sigset, NULL);

    printf("sleeping after block\n");
    sleep(3);
    printf("wake up\n");

    sigprocmask(SIG_UNBLOCK, &sigset, NULL);

    printf("sleeping after unblocking\n");
    sleep(3);
    printf("done\n");
    
    return 0;
}
