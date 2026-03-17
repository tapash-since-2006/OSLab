#include <stdio.h>
#include <stdlib.h> // Required for malloc
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

struct msg {
    long type;
    int pid;
    int operation;
    int a;
    int b;
    int result;
};

int main() {
    int msgid;
    struct msg *m = (struct msg *)malloc(sizeof(struct msg)); // Allocate memory

    msgid = msgget(1234, 0666 | IPC_CREAT);

    m->type = 1;
    m->pid = getpid();

    printf("Client PID: %d\n", m->pid);
    printf("Enter two numbers: ");
    scanf("%d %d", &m->a, &m->b);

    printf("Select Operation\n1.Add\n2.Subtract\n3.Multiply\n4.Divide\n");
    scanf("%d", &m->operation);

    // Note: We pass 'm' directly because it is already a pointer
    msgsnd(msgid, m, sizeof(struct msg) - sizeof(long), 0);

    printf("Request sent to server\n");

    // Receiving back based on PID
    msgrcv(msgid, m, sizeof(struct msg) - sizeof(long), m->pid, 0);

    printf("Result from server = %d\n", m->result);

    free(m); // Always clean up dynamic memory
    return 0;
}