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

    printf("Server Running...\n");

    while(1)
    {
        msgrcv(msgid,&m,sizeof(m)-sizeof(long),1,0);

        printf("Request from Client PID: %d\n",m.pid);

        switch(m.operation)
        {
            case 1:
                m.result = m.a + m.b;
                break;

            case 2:
                m.result = m.a - m.b;
                break;

            case 3:
                m.result = m.a * m.b;
                break;

            case 4:
                m.result = m.a / m.b;
                break;

            default:
                printf("Invalid Operation\n");
        }

        m.type = m.pid;

        msgsnd(msgid,&m,sizeof(m)-sizeof(long),0);

        printf("Result sent to Client %d\n",m.pid);
    }

    return 0;
}