#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>

#define BUF_SIZE 20

void print(char *tag) {
    char* msg = (char*)calloc(strlen(tag) + BUF_SIZE, sizeof(char));
    strcpy(msg, tag);
    struct flock wlock = {0};

    while(1) {
        char buf[BUF_SIZE] = {0};

        wlock.l_type = F_WRLCK;
        wlock.l_whence = SEEK_SET;

        // rlock.l_type = F_RDLCK;
        // rlock.l_whence = SEEK_SET;

        fcntl(STDOUT_FILENO, F_SETLK, &wlock);
        
        int n = read(STDIN_FILENO, buf, BUF_SIZE);
        write(STDOUT_FILENO, strcat(msg, buf), sizeof(buf) + n);
        
        wlock.l_type = F_UNLCK;
        fcntl(STDOUT_FILENO, F_SETLK, &wlock);

        strcpy(msg, tag);
    }

    free(msg);
}

int main() {
    int pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid > 0) {
        // parent
        print("parent: ");
    } else {
        // child
        print("child: ");
    }

    return 0;
}