#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "program gets msgid\n");
        return 1;
    }

    int msgid = atoi(argv[1]);
    if (msgid <= 0) {
        fprintf(stderr, "invalid msgid\n");
        return 1;
    }

    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        switch (errno) {
            case EINVAL:
                fprintf(stderr, "message queue does not exist or has an invalid ID\n");
                break;
            case EPERM:
                fprintf(stderr, "not valid permissions\n");
                break;
            case EACCES:
                fprintf(stderr, "no access to queue\n");
                break;
            case EIDRM:
                fprintf(stderr, "queue has already been deleted\n");
                break;
            default:
                perror("deleting the message queue\n");
        }
        return 1;
    }

    printf("message queue %d successfully removed\n", msgid);
    return 0;
}