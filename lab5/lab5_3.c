#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

typedef struct Message {
    long type;
    char data[100];
} Message;

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "program gets msgid and msgtype\n");
        return 1;
    }

    int msgid = atoi(argv[1]);
    long msgtype = atol(argv[2]);
    if (msgid <= 0) {
        fprintf(stderr, "invalid msgid\n");
        return 1;
    }

    Message msg;
    if (msgrcv(msgid, &msg, sizeof(msg), msgtype, IPC_NOWAIT) == -1) {
        if (errno == ENOMSG) {
            fprintf(stderr, "no messages of type %ld in the queue\n", msgtype);
        } else {
            perror("msgrcv");
        }        
        return 1;
    }
    printf("read message \"%s\"\n", msg.data);

    return 0;
}