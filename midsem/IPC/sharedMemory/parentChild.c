#include<stdio.h>
#include<stdlib.h>
#include<sys/shm.h>
#include<unistd.h>

struct data{
  char letter;
  int flag;
};


int main(){
  int shmid;
  struct data* shared;

  shmid=shmget(1234, sizeof(struct data), 0666|IPC_CREAT);
  shared=(struct data*) shmat(shmid, NULL,0);

  shared->flag=0;
  
  int pid=fork();

  if(pid==0){
    while(1){
      if(shared->flag==1){
        char ch=shared->letter;
        ch=ch+1;
        shared->letter=ch;
        sleep(3);
        shared->flag=2;
      }
    }
  }else{
    char input;
    while(1){
      printf("Enter the alphabet:");
      scanf(" %c", &input);

      shared->letter=input;
      shared->flag=1;

      while(shared->flag!=2);

      printf("Next alphabet from the child: %c \n", shared->letter);

      shared->flag=0;
    }

    shmdt(shared); // detaching
    shmctl(shmid, IPC_RMID, NULL); //deleting the shared memory
    return 0;
  }
}