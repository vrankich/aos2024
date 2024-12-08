#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int p[2];

    if (pipe(p) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // pid1 -> proc for command who
    int pid1 = fork();
    if (pid1 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        // child
        close(p[0]);
        
        // stdout -> channel 
        dup2(p[1], STDOUT_FILENO);
        close(p[1]);
        
        execlp("who", "who", NULL);
    }
    // pid2 -> proc for command wc -l
    int pid2 = fork();
    if (pid2 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0) {
        // child
        close(p[1]);        

        dup2(p[0], STDIN_FILENO);        
        close(p[0]);
        
        execlp("wc", "wc", "-l", NULL);
    }

    // close pipe in parent proc
    close(p[0]);
    close(p[1]);

    // wait for child procs
    wait(NULL);
    wait(NULL);

    return 0;
}