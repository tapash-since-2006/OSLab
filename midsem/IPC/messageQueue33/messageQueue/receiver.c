#include<stdio.h>
#include<sys/ipc.h>
#include<sys/msg.h>

struct msg
{
    long type;
    int num;
};

int main(){
  int msgid, n, rev=0, temp;

  struct msg m;

  msgid=msgget(1234,0666 | IPC_CREAT);

  msgrcv(msgid, &m, sizeof(m.num),1,0);

  n=m.num;
  temp=n;

  while(n>0){
    rev=rev*10+n%10;
    n=n/10;
  }

  if(rev==temp) printf("Palindrome\n");
  else printf("Not a Palindrome\n");

  return 0;
}