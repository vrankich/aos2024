#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("program %s gets 1 argument\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 2;
    }

    size_t size = lseek(fd, 0, SEEK_END);
    if (size > 0) {
        char buf;
        do {
            if (read(fd, &buf, 1) == -1) {
                perror("read file");
                close(fd);
                return 3;
            }
            printf("%c", buf);            
        } while (lseek(fd, -2, SEEK_CUR) >= 0);
        printf("\n");
    }
    close(fd);

    return 0;
}