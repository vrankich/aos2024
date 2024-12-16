#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>


#define MSG_SIZE 80

typedef struct Message {
    long type;
    char data[MSG_SIZE + 1];
} Message;

int msgid;

void handler(int sig) {
    printf("server received signal %d\n", sig);
    msgctl(msgid, IPC_RMID, 0);
    exit(sig);
}

int main(int argc, char *argv[]) {
    signal(SIGINT, handler);

    key_t key = ftok(argv[0], 'v');
    msgid = msgget(key, IPC_CREAT | 0666);

    Message msg;
    int pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid > 0) {
        // parent
        while (1) {
            msg.type = 1;
            printf("PID: %d\n", getpid());
            sleep(2);
            msgsnd(msgid, &msg, sizeof(msg), 0);
            msgrcv(msgid, &msg, sizeof(msg), 2, 0);
        }
    } else {
        //child
        while (1) {
            printf("PID: %d\n", getpid());
            msgrcv(msgid, &msg, sizeof(msg), 1, 0);
            sleep(2);
            msg.type = 2;
            msgsnd(msgid, &msg, sizeof(msg), 0);
        }
    }

    msgctl(msgid, IPC_RMID, 0);

    return 0;
}