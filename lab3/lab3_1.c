#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>

void handler(int s) {
    if (s == SIGSEGV) {
        printf("seg fault :)\n");
        exit(1);            
    } else {
        signal(s, SIG_DFL); // default
    }
}

int main() {
    signal(SIGSEGV, handler);

    int nums[1];
    nums[5] = 3;

    return 0;
}