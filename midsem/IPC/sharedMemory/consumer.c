#include<stdio.h>
#include<sys/shm.h>
#include<unistd.h>

struct data{
    int flag;
    char text[100];
};


int main(){
    int shmid;
    struct data *shared;

    shmid=shmget(1234, sizeof(struct data), 0666);

    shared=(struct data *) shmat(shmid, NULL, 0);

    while(1){
        if(shared->flag==1){
            printf("Received: %s\n", shared->text);
            shared->flag=0;
        }

        sleep(1);
    }

    shmdt(shared); // detaching the shared memory
    shmctl(shmid, IPC_RMID, NULL); // closing the shared memory

    return 0;
}