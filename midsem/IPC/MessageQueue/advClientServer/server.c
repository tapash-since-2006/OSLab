#include <stdio.h>
#include <stdlib.h>
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
    struct msg *m = (struct msg *)malloc(sizeof(struct msg));

    msgid = msgget(1234, 0666 | IPC_CREAT);
    printf("Server Running...\n");

    while(1) {
        // Blocks until a message with type 1 arrives
        msgrcv(msgid, m, sizeof(struct msg) - sizeof(long), 1, 0);

        printf("Request from Client PID: %d\n", m->pid);

        switch(m->operation) {
            case 1: m->result = m->a + m->b; break;
            case 2: m->result = m->a - m->b; break;
            case 3: m->result = m->a * m->b; break;
            case 4: 
                if(m->b != 0) m->result = m->a / m->b; 
                else m->result = 0; // Simple error handling
                break;
            default: printf("Invalid Operation\n");
        }

        m->type = m->pid; // Address it back to the specific client
        msgsnd(msgid, m, sizeof(struct msg) - sizeof(long), 0);
        printf("Result sent to Client %d\n", m->pid);
    }

    free(m);
    return 0;
}