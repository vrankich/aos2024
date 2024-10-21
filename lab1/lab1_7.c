#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "program %s gets 2 arguments\n", argv[0]);
        return 1;
    }

    struct stat statbuf = {0};
    if (stat(argv[1], &statbuf) == -1) {
        perror("stat error");
        return 2;
    }

    printf("ID of device containing file: %lu\n", statbuf.st_dev);
    printf("Inode: %lu\n", statbuf.st_ino);
    printf("File permissions mask: %o\n", statbuf.st_mode);
    printf("Owner ID: %u\n", statbuf.st_uid);
    printf("Owner group ID: %u\n", statbuf.st_gid);
    printf("Device ID (0 if not device): %lu\n", statbuf.st_rdev);
    printf("Total size [bytes]: %lu\n", statbuf.st_size);
    printf("I/O block size: %lu\n", statbuf.st_blksize);
    printf("Blocks: %lu\n", statbuf.st_blocks);

    double atime_minutes = difftime(time(NULL), statbuf.st_atime) / 60.0;
    printf("Time of last access: %.2lf min\n", atime_minutes);

    double mtime_minutes = difftime(time(NULL), statbuf.st_mtime) / 60.0;
    printf("Time of last modification: %.2lf min\n", mtime_minutes);
    
    double ctime_minutes = difftime(time(NULL), statbuf.st_ctime) / 60.0;
    printf("Time of last status change: %.2lf min\n", ctime_minutes);

    return 0;
}