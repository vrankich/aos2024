#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

#define MSG_SIZE 80

typedef struct Message {
    long type;
    long from;
    char data[MSG_SIZE + 1];
} Message;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "program gets name of the server\n");
        return 1;
    }

    char *server_fname = argv[1];
    
    key_t key = ftok(server_fname, 'v');
    if (key == -1) {
        perror("ftok");
        return 1;
    }

    int msgid = msgget(key, 0666);
    if (msgid == -1) {
        perror("msgget");
        return 1;
    }

    Message request, response;
    while (1) {
        request.type = 1;
        request.from = getpid();
        strcpy(request.data, "hello from client");
        if (msgsnd(msgid, &request, sizeof(request), 0) == -1) {
            perror("msgsnd");
            return 1;
        }
        printf("client sent a message to server\n");

        if (msgrcv(msgid, &response, sizeof(response), getpid(), 0) == -1) {
            perror("msgrcv");
            return 1;
        } else {
            printf("client recieved a message \"%s\"\n", response.data);
        }

        sleep(2);
    }

    return 0;
}