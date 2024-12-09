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
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("open file");
        return 1;
    }

    char buf[81] = {0};
    int n = read(fd, buf, sizeof(buf));
    printf("read from \"%s\": %s\n", filename, buf);

    close(fd);

    return 0;
}
