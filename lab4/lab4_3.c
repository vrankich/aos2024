#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

#define BUFF_SIZE 81 

int main() {
    int p1[2]; // parent[1] -> child[0]
    int p2[2]; //child[1] -> parent[0]
    
    if (pipe2(p1, O_NONBLOCK) == -1) {
        perror("pipe1");
        return EXIT_FAILURE;
    }
    if (pipe2(p2, O_NONBLOCK) == -1) {
        perror("pipe2");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid > 0) {
        // parent
        close(p1[1]);
        close(p2[0]);

        int n;
        do {
            char buf[BUFF_SIZE];
            n = read(p1[0], buf, BUFF_SIZE);
            if (n > 0) {
                printf("Parent read: %s", buf);
                write(p2[1], buf, n);
            }
        } while(n);

        close(p1[0]);
        close(p2[1]);
    } else {
        // child
        close(p1[0]);
        close(p2[1]);

        int n_w = 0, n_r = 0;
        char buf[BUFF_SIZE];
        while(1) {
            memset(buf, 0, BUFF_SIZE);
            n_w = read(STDIN_FILENO, buf, 80);
            write(p1[1], buf, n_w);
            memset(buf, 0, BUFF_SIZE);
            n_r = read(p2[0], buf, BUFF_SIZE);
            if (n_r > 0) {
                printf("Child read: %s", buf);
            }
        }

        close(p1[1]);
        close(p2[0]);
    }

    return 0;
}