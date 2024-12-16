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
    char data[MSG_SIZE + 1];
} Message;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "program gets name of the server\n");
        return 1;
    }

    char *server_fname = argv[1];
    
    key_t key_request = ftok(server_fname, 'a');
    if (key_request == -1) {
        perror("ftok");
        return 1;
    }

    int msgid_request = msgget(key_request, 0666);
    if (msgid_request == -1) {
        perror("msgget");
        return 1;
    }

    key_t key_response = ftok(server_fname, 'b');
    if (key_response == -1) {
        perror("ftok");
        return 1;
    }

    int msgid_response = msgget(key_response, 0666);
    if (msgid_response == -1) {
        perror("msgget");
        return 1;
    }

    Message request, response;
    while (1) {
        request.type = getpid();
        strcpy(request.data, "hello from client");
        if (msgsnd(msgid_request, &request, sizeof(request), 0) == -1) {
            perror("msgsnd");
            return 1;
        }
        printf("client sent a message to server\n");

        if (msgrcv(msgid_response, &response, sizeof(response), 0, 0) == -1) {
            perror("msgrcv");
            return 1;
        } else {
            printf("client recieved a message \"%s\"\n", response.data);
        }

        sleep(2);
    }

    return 0;
}