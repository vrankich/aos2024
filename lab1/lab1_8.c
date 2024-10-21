#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int copy(int fd_src, int fd_dst) {
    char buf[1024];
    size_t n = 0, res = 0;

    while ((n = read(fd_src, buf, sizeof(buf))) > 0) {
        if (write(fd_dst, buf, n) != n) {
            return -1;
        }
        res += n;
    }

    return res;
}

int main(int argc, char *argv[]) {
    char *src = NULL, *dst = NULL;
    int fd_dst, fd_src;
    
    if (argc == 1) {
        fd_src = STDIN_FILENO;
        fd_dst = STDOUT_FILENO;
    } else if (argc == 3) {
        src = argv[1];
        fd_src = open(src, O_RDONLY);
        if (fd_src == -1) {
            perror("open source file");
            return 1;
        }
    
        dst = argv[2];
        fd_dst = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd_dst == -1) {
            perror("open destination file");
            return 2;
        }
    } else {
        fprintf(stderr, "usage: %s <src file> <dst file>\n", argv[0]);
        return 3;
    }

    size_t bytes_copied = copy(fd_src, fd_dst);
    if (bytes_copied < 0) {
        perror("copy error");
        close(fd_src);
        close(fd_dst);
        return 4;
    }
    printf("successfully copied %ld bytes\n", bytes_copied);

    close(fd_src);
    close(fd_dst);

    return 0;
}