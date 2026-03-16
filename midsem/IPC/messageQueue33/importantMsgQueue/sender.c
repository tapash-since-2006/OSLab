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

    int choice;

    printf("1.Send Number\n");
    printf("2.Send String\n");
    printf("3.Send Array\n");
    printf("Enter choice: ");
    scanf("%d",&choice);

    if(choice==1)
    {
        m.type = 1;

        printf("Enter a number: ");
        scanf("%d",&m.num);

        msgsnd(msgid,&m,sizeof(m)-sizeof(long),0);

        printf("Number Sent\n");
    }

    else if(choice==2)
    {
        m.type = 2;

        printf("Enter a string: ");
        scanf("%s",m.text);

        msgsnd(msgid,&m,sizeof(m)-sizeof(long),0);

        printf("String Sent\n");
    }

    else if(choice==3)
    {
        m.type = 3;

        printf("Enter 5 numbers:\n");

        for(i=0;i<5;i++)
            scanf("%d",&m.arr[i]);

        msgsnd(msgid,&m,sizeof(m)-sizeof(long),0);

        printf("Array Sent\n");
    }

    return 0;
}