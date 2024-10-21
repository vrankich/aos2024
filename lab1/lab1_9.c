#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int copy() {
    char buf[1024];
    size_t n = 0, res = 0;

    while ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
        if (write(STDOUT_FILENO, buf, n) != n) {
            return -1;
        }
        res += n;
    }

    return res;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s <src file> <dst file>\n", argv[0]);
        return 1;
    }

    char *src = argv[1];
    int fd_src = open(src, O_RDONLY);
    if (fd_src == -1) {
        perror("open source file");
        return 1;
    }
    if (dup2(fd_src, STDIN_FILENO) == -1) {
        perror("source");
        close(fd_src);
        return 2;
    }

    char *dst = argv[2];
    int fd_dst = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_dst == -1) {
        perror("open destination file");
        return 3;
    }
    if (dup2(fd_dst, STDOUT_FILENO) == -1) {
        perror("destination");
        close(fd_dst);
        return 4;
    }

    size_t bytes_copied = copy(fd_src, fd_dst);
    if (bytes_copied < 0) {
        perror("copy error");
        close(fd_src);
        close(fd_dst);
        return 5;
    }
    
    close(fd_src);
    close(fd_dst);

    return 0;
}