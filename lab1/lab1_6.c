#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int write_to_file(int fd, char *msgs[]) {
if (write(fd, msgs[2], strlen(msgs[2])) < 0) {
        perror("couldn't write messageto the file");
        return -1;
    }

    // move the file pointer to the start of the file
    if (lseek(fd, 0, SEEK_SET) == -1) {
        perror("couldn't move the file pointer to the start of the file");
        return -2;
    }

    // overwrite
    if (write(fd, msgs[0], strlen(msgs[0])) < 0) {
        perror("couldn't write message to the file");
        return -3;
    }

    // add new message
    if (write(fd, msgs[2], strlen(msgs[2])) < 0) {
        perror("couldn't write message to the file");
        return -4;
    }

    // move the file pointer to the start of the last message
    if (lseek(fd, -strlen(msgs[2]), SEEK_CUR) < 0) {
        perror("couldn't move the file pointer");
        return -5;
    }

    // overwrite the last message
    if (write(fd, msgs[1], strlen(msgs[1])) < 0) {
        perror("couldn't write message to the file");
        return -6;
    }

    // write last message to the file
    if (write(fd, msgs[2], strlen(msgs[2])) < 0) {
        perror("couldn't write message to the file");
        return -7;
    }

    // move the file pointer to the start of the last message
    if (lseek(fd, 15000, SEEK_CUR) < 0) {
        perror("couldn't move the file pointer");
        return -5;
    }
    write(fd, msgs[2], strlen(msgs[2]));

    return 1;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "program %s gets 2 arguments\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    int fd = open(filename, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("couldn't open the file");
        return 2;
    }

    char *messages[] = {"lera1\n", "lera2\n", "lera3\n"};
    if (write_to_file(fd, messages) < -1) {
        perror("couldn't write messages to the file");
        if (close(fd) == -1) {
            perror("couldn't close the file");
            return 3;
        }
        return 4;
    }
    fprintf(stdout, "messages were successfully written to the file %s\n", filename);
    if (close(fd) == -1) {
        perror("couldn't close the file");
        return 5;
    }

    return 0;
}