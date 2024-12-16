#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <signal.h>

#define MSG_SIZE 80

int msgid;

typedef struct Message {
    long type;
    long from;
    char data[MSG_SIZE + 1];
} Message;

void handler(int sig) {
    printf("server received signal %d\n", sig);
    msgctl(msgid, IPC_RMID, NULL);
    exit(sig);
}

int main(int argc, char *argv[]) {
    signal(SIGINT, handler);

    key_t key = ftok(argv[0], 'v');
    if (key == -1) {
        perror("ftok");
        return 1;
    }

    msgid = msgget(key, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("msgget");
        return 1;
    }

    Message request, response;
    while (1) {
        if (msgrcv(msgid, &request, sizeof(request), 1, 0) == -1) {
            perror("msgrcv");
            continue;
        } else {
            printf("server recieved message \"%s\"\n", request.data);
        }

        response.type = request.from;
        strcpy(response.data, "hello from server");
        if (msgsnd(msgid, &response, sizeof(response), 0) == -1) {
            perror("msgsnd");
            continue;
        }
        printf("server sent a message to client\n");
    }

    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}
