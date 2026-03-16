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

    int sum[24]={0};
    int count[24]={0};

    msgid = msgget((key_t)1234,0666);

    for(int i=0;i<6;i++)
    {
        msgrcv(msgid,&data,sizeof(data)-sizeof(long),1,0);

        sum[data.hour] += data.users;
        count[data.hour]++;
    }

    printf("\nAverage users per hour:\n");

    for(int i=0;i<24;i++)
    {
        if(count[i] != 0)
        {
            printf("Hour %d : %.2f users\n",i,(float)sum[i]/count[i]);
        }
    }

    msgctl(msgid,IPC_RMID,0);

    return 0;
}