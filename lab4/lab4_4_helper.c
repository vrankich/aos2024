#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    int n;
    do {
        char buf[80] = {0};
        n = read(STDIN_FILENO, buf, 80);
        write(STDOUT_FILENO, buf, n);
    } while(n);

    return 0;
}