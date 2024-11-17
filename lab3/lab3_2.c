#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

void handler(int s) {
    if (s == SIGINT) {
        printf("caught SIGINT\n");
        exit(0);
    }
}

int main() {
    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("signal SIGINT");
    }
    
    while(1) {
        sleep(1);
    }

    return 0;
}