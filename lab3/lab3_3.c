#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

int main() {
    signal(SIGCHLD, SIG_IGN);

    for (int i = 0; i < 10; i++) {
        if (fork() == 0) {
            printf("child PID: %d\n", getpid());
            sleep(5);
            return 0;
        }
    }

    while(1) {
        sleep(1);
    }

    return 0;
}