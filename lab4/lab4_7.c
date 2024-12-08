#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "filename expected\n");
        return 1;
    }

    const char* filename = argv[1];
    int pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    char *msgs[] = {"1", "2", "3", "4", "5"};
    if (pid > 0) {
        // parent
        int fd = open(filename, O_WRONLY | O_CREAT, 0666);
        for (int i = 0; i < 100; i++) {
            // printf("write");
            write(fd, msgs[i % 5], 1);
        }
        // write(fd, "\n", 1);
        close(fd);
    } else {
        // child
        int fd = open(filename, O_RDONLY);
        char buf[1];
        while(read(fd, buf, 1) > 0) {
            printf("%s", buf);
        }
        close(fd);
    }

    return 0;
}