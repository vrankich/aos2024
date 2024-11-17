#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char * argv[]) {
    if (argc <= 2) {
        fprintf(stderr, "usage: %s command [args...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    char command[256] = "";
    strcpy(command, argv[1]); // команда
    for (int i = 2; i < argc; ++i) {
        strcat(command, " ");
        strcat(command, argv[i]);
    }

    system(command);
    
    return EXIT_SUCCESS;
}