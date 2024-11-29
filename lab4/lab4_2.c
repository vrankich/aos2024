#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main() {
    int p[2]; // parent[1] -> child[0], child[1] -> parent[0]
    
    if (pipe(p) == -1) {
        perror("pipe p");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return EXIT_FAILURE;
    }
    if (pid > 0) {
        // parent
        close(p[1]); // close write

        int n;
        do {
            char buf[80] = {0};
            n = read(p[0], buf, 80);
            printf("%s", buf);
        } while(n);

        close(p[0]);
        close(p[1]);
    } else {
        // child
        close(p[0]); // close read

        int n;
        while(1) {
            char buf[80] = {0};
            n = read(STDIN_FILENO, buf, 80);
            write(p[1], buf, n);
        }
    }

    return 0;
}