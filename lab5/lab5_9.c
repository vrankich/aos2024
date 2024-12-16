#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <signal.h>

#define MSG_SIZE 80

int msgid_response;
int msgid_request;

typedef struct Message {
    long type;
    char data[MSG_SIZE + 1];
} Message;

void handler(int sig) {
    printf("server received signal %d\n", sig);
    msgctl(msgid_response, IPC_RMID, 0);
    msgctl(msgid_request, IPC_RMID, 0);
    exit(sig);
}

int main(int argc, char *argv[]) {
    signal(SIGINT, handler);

    key_t key_request = ftok(argv[0], 'a');
    if (key_request == -1) {
        perror("ftok");
        return 1;
    }

    msgid_request = msgget(key_request, IPC_CREAT | 0666);
    if (msgid_request == -1) {
        perror("msgget");
        return 1;
    }

    key_t key_response = ftok(argv[0], 'b');
    if (key_response == -1) {
        perror("ftok");
        return 1;
    }

    msgid_response = msgget(key_response, IPC_CREAT | 0666);
    if (msgid_response == -1) {
        perror("msgget");
        return 1;
    }

    Message request, response;
    while (1) {
        if (msgrcv(msgid_request, &request, sizeof(request), 0, 0) == -1) {
            perror("msgrcv");
            continue;
        } else {
            printf("server recieved message \"%s\"\n", request.data);
        }

        response.type = request.type;
        strcpy(response.data, "hello from server");
        if (msgsnd(msgid_response, &response, sizeof(response), 0) == -1) {
            perror("msgsnd");
            continue;
        }
        printf("server sent a message to client\n");
    }

    msgctl(msgid_response, IPC_RMID, 0);
    msgctl(msgid_request, IPC_RMID, 0);
    
    return 0;
}
