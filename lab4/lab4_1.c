#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define BUFFER_SIZE 81

int main() {
    int pipe1[2]; // parent[1] -> child[0]
    int pipe2[2]; // child[1] -> parent[0]
    
    if (pipe(pipe1) == -1) {
        perror("pipe1");
        return EXIT_FAILURE;
    }

    if (pipe(pipe2) == -1) {
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
        close(pipe1[0]); // close read
        close(pipe2[1]); // close write

        const char *msg = "hiiiiiii";
        write(pipe1[1], msg, strlen(msg) + 1); // send msg to child

        char buffer[BUFFER_SIZE];
        read(pipe2[0], buffer, BUFFER_SIZE); // read msg from child
        printf("Parent received message: \"%s\"\n", buffer);

        close(pipe1[1]);
        close(pipe2[0]);
    } else {
        // child
        close(pipe1[1]);
        close(pipe2[0]);

        char buffer[BUFFER_SIZE];
        read(pipe1[0], buffer, BUFFER_SIZE);
        printf("Child received message: \"%s\"\n", buffer);

        const char *msg = "ohhh";
        write(pipe2[1], msg, strlen(msg) + 1);

        close(pipe1[0]);
        close(pipe2[1]);
    }

    return 0;
}