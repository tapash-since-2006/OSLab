#include<stdio.h>
#include<sys/ipc.h>
#include<sys/msg.h>

struct msg{
  long type;
  int num;
};

int main(){
  int msgid;
  struct msg m;
  
  msgid=msgget(1234,0666 | IPC_CREAT);

  printf("enter a number: ");
  scanf("%d", &m.num);

  m.type=1;

  msgsnd(msgid, &m, sizeof(m.num), 0);

  printf("Number sent\n");

  return 0;
}