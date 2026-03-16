#include<stdio.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<unistd.h>

struct msg
{
    long type;
    int pid;
    int operation;
    int a;
    int b;
    int result;
};

int main()
{
    int msgid;
    struct msg m;

    msgid = msgget(1234,0666|IPC_CREAT);

    m.type = 1;

    m.pid = getpid();

    printf("Client PID: %d\n",m.pid);

    printf("Enter two numbers: ");
    scanf("%d %d",&m.a,&m.b);

    printf("Select Operation\n");
    printf("1.Add\n2.Subtract\n3.Multiply\n4.Divide\n");

    scanf("%d",&m.operation);

    msgsnd(msgid,&m,sizeof(m)-sizeof(long),0);

    printf("Request sent to server\n");

    msgrcv(msgid,&m,sizeof(m)-sizeof(long),m.pid,0);

    printf("Result from server = %d\n",m.result);

    return 0;
}