#include<stdio.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<string.h>

struct msg
{
    long type;
    int num;
    char text[100];
    int arr[5];
};

int main()
{
    int msgid,i;
    struct msg m;

    msgid = msgget(1234,0666|IPC_CREAT);

    printf("Waiting for messages...\n");

    while(1)
    {
        msgrcv(msgid,&m,sizeof(m)-sizeof(long),0,0);

        if(m.type==1)
        {
            printf("Received Number: %d\n",m.num);
        }

        else if(m.type==2)
        {
            printf("Received String: %s\n",m.text);
        }

        else if(m.type==3)
        {
            printf("Received Array: ");

            for(i=0;i<5;i++)
                printf("%d ",m.arr[i]);

            printf("\n");
        }
    }

    return 0;
}