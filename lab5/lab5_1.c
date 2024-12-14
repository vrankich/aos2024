#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <string.h>

typedef struct Message {
    long type;
    char data[80];
} Message;

int main() {
    char *fname = "queue.txt";

    key_t key = ftok(fname, 'v');
    if (key == -1) {
        perror("ftok");
        return 1;
    }

    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        return 1;
    }
    printf("msgid: %d\n", msgid);

    int types[] = {1, 2, 3, 4, 5};
    const char* messages[] = {"msg 1", "msg 2", "msg 3", "msg 4", "msg 5"};

    struct Message msg;
    for (int i = 0; i < 5; i++) {
        msg.type = types[i];
        strcpy(msg.data, messages[i]);
        if (msgsnd(msgid, &msg, sizeof(msg), 0) == -1) {
            perror("msgsend");
            return 1;
        }
    }

    struct msqid_ds buf = {0};
    msgctl(msgid, IPC_STAT, &buf);

    struct ipc_perm perm = buf.msg_perm;
    printf("Key: %d\n", perm.__key);
    printf("Owner UID: %d\n", perm.uid);
    printf("Creator UID: %d\n", perm.cuid);
    printf("Permissions: %o\n", perm.mode);
    printf("Size: %lu\n", buf.__msg_cbytes);
    printf("Max size: %lu\n", buf.msg_qbytes);
    printf("Messages in queue: %lu\n", buf.msg_qnum);
    printf("PID last sender: %d\n", buf.msg_lspid);
    printf("PID last receiver: %d\n", buf.msg_lrpid);
    printf("Time of last send: %ld\n", buf.msg_stime);
    printf("Time of last read: %ld\n", buf.msg_rtime);
    printf("Time of last change: %ld\n", buf.msg_ctime);

    // msgctl(msgid, IPC_RMID, NULL);

    return 0;
}
