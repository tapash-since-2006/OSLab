#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<unistd.h>

struct msg {
    long type;
    int a;
    int b;
    int result;
};

int main()
{
    int msgid;
    struct msg *message;

    // if you dont do the malloc thing than it will give the error
    message = malloc(sizeof(struct msg));

    msgid = msgget(1234, 0666 | IPC_CREAT);

    int pid1 = fork();

    if(pid1 == 0)
    {
        // CHILD 1 → SUM
        // here message is memory hence no need of the &message
        msgrcv(msgid, message, sizeof(struct msg)-sizeof(long), 1, 0);

        message->result = message->a + message->b;
        message->type = 4;

        msgsnd(msgid, message, sizeof(struct msg)-sizeof(long), 0);
        exit(0);
    }

    int pid2 = fork();

    if(pid2 == 0)
    {
        // CHILD 2 → SUBTRACTION
        msgrcv(msgid, message, sizeof(struct msg)-sizeof(long), 2, 0);

        message->result = message->a - message->b;
        message->type = 5;

        msgsnd(msgid, message, sizeof(struct msg)-sizeof(long), 0);
        exit(0);
    }

    int pid3 = fork();

    if(pid3 == 0)
    {
        // CHILD 3 → MULTIPLICATION
        msgrcv(msgid, message, sizeof(struct msg)-sizeof(long), 3, 0);

        message->result = message->a * message->b;
        message->type = 6;

        msgsnd(msgid, message, sizeof(struct msg)-sizeof(long), 0);
        exit(0);
    }

    // PARENT
    printf("Enter two numbers: ");
    scanf("%d %d", &message->a, &message->b);

    message->type = 1;
    msgsnd(msgid, message, sizeof(struct msg)-sizeof(long), 0);

    message->type = 2;
    msgsnd(msgid, message, sizeof(struct msg)-sizeof(long), 0);

    message->type = 3;
    msgsnd(msgid, message, sizeof(struct msg)-sizeof(long), 0);

    // Receive results
    msgrcv(msgid, message, sizeof(struct msg)-sizeof(long), 4, 0);
    printf("Sum = %d\n", message->result);

    msgrcv(msgid, message, sizeof(struct msg)-sizeof(long), 5, 0);
    printf("Subtraction = %d\n", message->result);

    msgrcv(msgid, message, sizeof(struct msg)-sizeof(long), 6, 0);
    printf("Multiplication = %d\n", message->result);

    msgctl(msgid, IPC_RMID, NULL);

    free(message);

    return 0;
}