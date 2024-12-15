#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <unistd.h>

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
    key_t key = ftok(server_fname, 'a');
    if (key == -1) {
        perror("ftok");
        return 1;
    }

    int server_msgid = msgget(key, 0666);
    if (server_msgid == -1) {
        perror("msgget for server");
        return 1;
    }

    int client_msgid = msgget(IPC_PRIVATE, 0666 | IPC_CREAT);
    if (client_msgid == -1) {
        perror("msgget for client");
        return 1;
    }

    Message request, response;
    while (1) {
        request.type = client_msgid;
        strcpy(request.data, "hello from client");
        if (msgsnd(server_msgid, &request, sizeof(request.data), 0) == -1) {
            perror("msgsnd");
            msgctl(client_msgid, IPC_RMID, NULL);
            return 1;
        }
        printf("client sent a message to server\n");

        if (msgrcv(client_msgid, &response, sizeof(response.data), 0, 0) == -1) {
            perror("msgrcv");
            msgctl(client_msgid, IPC_RMID, NULL);
            return 1;
        } else {
            printf("client recieved a message \"%s\"\n", response.data);
        }

        sleep(2);
    }

    msgctl(client_msgid, IPC_RMID, NULL);

    return 0;
}