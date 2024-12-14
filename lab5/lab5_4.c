#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define MAX_MSG_SIZE 256

typedef struct Message {
    long type;
    char data[MAX_MSG_SIZE];
} Message;

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "program gets msgid and msgtype\n");
        return 1;
    }

    int msgid = atoi(argv[1]);
    if (msgid <= 0) {
        fprintf(stderr, "invalid msgid\n");
        return 1;
    }
    
    long msgtype = atol(argv[2]);
    int nbytes = atoi(argv[3]);
    if (nbytes > MAX_MSG_SIZE) {
        fprintf(stderr, "invalid msg size\n");
        return 1;
    }

    Message msg;

    ssize_t bytes_read = msgrcv(msgid, &msg, sizeof(msg), msgtype, IPC_NOWAIT);
    if (bytes_read == -1) {
        if (errno == ENOMSG) {
            fprintf(stderr, "no messages of type %ld in the queue\n", msgtype);
        } else {
            perror("msgrcv");
        }        
        return 1;
    }

    msg.data[nbytes] = '\0';
    printf("read message \"%s\"\n", msg.data);

    return 0;
}