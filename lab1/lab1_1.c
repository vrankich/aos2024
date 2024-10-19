#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

int make_file_exec(const char *file) {
	int res = chmod(file, 0111);
	if (res == -1) {
		fprintf(stderr, "chmod failed with code %d\n", errno);
		fprintf(stderr, "chmod failed: %s\n", strerror(errno));
		perror("chmod failed");
		return -1;
	}

	printf("chmod succeed\n");
	return 1;
}

int main(int argc, char *argv[]) {
	const char *filename = argv[1];
	if (make_file_exec(filename) <= 0) {
		return 1;
	}

	return 0;
}