#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "filename expected\n");
        return 1;
    }

    char *filename = argv[1];
    if (mkfifo(filename, 0777) == -1) {
        perror("mkfifo");
        return 1;
    }

    int fd = open(filename, O_WRONLY);
    if (fd < 0) {
        perror("open file");
        unlink(filename);
        return 1;
    }

    const char *msg = "hiii!";
    write(fd, msg, strlen(msg));
    
    close(fd);
    unlink(filename);

    return 0;
}