#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msg_st {
    long int msg_type;
    int users;
    int hour;
    int minute;
};

int main()
{
    int msgid;
    struct msg_st data;

    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);

    data.msg_type = 1;

    for(int i=0;i<6;i++)   // 6 readings (every 10 min for 1 hour)
    {
        printf("Enter hour minute and number of users: ");
        scanf("%d %d %d",&data.hour,&data.minute,&data.users);

        msgsnd(msgid,&data,sizeof(data)-sizeof(long),0);
    }

    printf("Data sent to message queue\n");

    return 0;
}