#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

void write_to_file(const int fd) {
	const uint len = 15;
	const char *msg = "lera\nlera\nlera\n";
	write(fd, msg, len);
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
	write_to_file(fd);

	fd = open(filename, O_RDONLY);
	char buf[1024];
	int r = read(fd, buf, 1024);
	if (r < 0) {
		fprintf(stderr, "coudn't read file %s\n", filename);
		return 4;
	}
	printf("Message:\n%s\n", buf);
	close(fd);

	fd = open(filename, O_RDWR);
	printf("fd = %d\n", fd);
	if (fd == -1) {
		fprintf(stderr, "coudn't open the file\n");
		return 5;
	}
	close(fd);

	return 0;
}