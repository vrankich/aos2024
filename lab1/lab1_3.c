#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int write_to_file(const int fd) {
	const uint len = 15;
	const char *msg = "lera\nlera\nlera\n";
	if (write(fd, msg, len) == -1) {
		fprintf(stderr, "coudn't write message to the file\n");
		return -1;
	}

	return 1;
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		fprintf(stderr, "%s needs 2 additional args\n", argv[0]);
		return 1;
	}

	const char *filename = argv[1];
	int filemask = strtol(argv[2], NULL, 0);
	if (filemask == LONG_MAX || filemask == LONG_MIN) {
		fprintf(stderr, "coudn't convert mask\n");
		return 2;
	}

	int fd = open(filename, O_WRONLY | O_CREAT, filemask);
	if (fd == -1) {
		fprintf(stderr, "coudn't create file %s with mask %d\n", filename, filemask);
		return 3;
	}

	if (write_to_file(fd) <= 0) {
		fprintf(stderr, "coudn't write to file %s\n", filename);
		return 4;
	}

	fd = open(filename, O_RDONLY);
	char buf[1024];
	int n = read(fd, buf, 1024);
	if (n < 0) {
		fprintf(stderr, "coudn't read file %s\n", filename);
		return 5;
	}
	printf("Message:\n%s\n", buf);

	if (close(fd) == -1) {
		fprintf(stderr, "coudn't close the file\n");
		return 6;
	}

	fd = open(filename, O_RDWR);
	printf("fd = %d\n", fd);
	if (fd == -1) {
		fprintf(stderr, "coudn't open the file\n");
		return 7;
	}
	if (close(fd) == -1) {
		fprintf(stderr, "coudn't close the file\n");
		return 6;
	}

	return 0;
}