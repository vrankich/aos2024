#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char * argv[]) {
    if (argc != 2) {
        printf("%s gets a file name\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    int fork_res = fork();
    if (fork_res == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }
    
    int fd_src = open(argv[1], O_RDONLY);
    if (fd_src == -1) {
        perror("open file");
        return EXIT_FAILURE;
    }

    // создание файлов для записи в обоих процессах
    char *filename = "";
    if (fork_res == 0) {
        filename = "child_file.txt";
    } else {
        filename = "parent_file.txt";
    }

    int fd_dst = open(filename, O_RDWR | O_CREAT, 0644);
    if (fd_dst == -1) {
        perror("open destination file");
        return EXIT_FAILURE;
    }

    size_t n = 0;
    char buf[256];
    do {
        n = read(fd_src, buf, sizeof(buf));
        if (n == -1) {
            perror("read from source file");
            return EXIT_FAILURE;
        }
        if (n == 0) {
            break;
        }

        if (write(fd_dst, buf, n) != n) {
            perror("write to destination file");
            return EXIT_FAILURE;
        }
        memset(buf, '\0', sizeof(buf));
    } while (n == sizeof(buf));
    close(fd_src); // закрытие файла для чтения

    lseek(fd_dst, 0, SEEK_SET);
    if (fork_res == 0) {
        printf("child\n");
    } else {
        int child_res;
        wait(&child_res); // ожидание завершения дочернего процесса
        printf("parent\n");
    }

    do {
        n = read(fd_dst, buf, sizeof(buf));
        if (n == -1) {
            perror("read from destination file");
            return EXIT_FAILURE;
        }
        if (n == 0) {
            break;
        }

        printf("%s", buf);
        memset(buf, '\0', sizeof(buf));
    } while (n == sizeof(buf));
    close(fd_dst); // закрытие файла для записи

    return EXIT_SUCCESS;
}