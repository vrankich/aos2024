#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <limits.h>
#include <time.h>

int main(int argc, char* argv[]) {
    int p[2];

    if (pipe(p) == -1) {
        perror("pipe p");
        return 1;
    }

    int pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid > 0) {
        close(p[1]);
        
        int n;
        char buf[80] = {0};
        do {
            memset(buf, 0, 80);
            n = read(p[0], buf, 80);
            printf("%s", buf);
        } while(n);
        
        close(p[0]);
    } else {
        close(p[0]);

        if (dup2(p[1], STDOUT_FILENO) < -1) {
            perror("dup2 for pipe p");
            return 1;
        }

        close(p[1]);
        execl("./lab4_helper.o", "./lab4_helper.o", NULL);
        
    }

    return 0;
}