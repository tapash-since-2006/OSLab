#include<stdio.h>
#include<sys/shm.h>

struct data{
    int flag;
    char text[100];
};

int main(){

    int shmid;
    struct data *shared;


    // here no need of malloc beacuse the shared autommatically does that hence no need
    shmid = shmget(1234, sizeof(struct data), 0666 | IPC_CREAT);

    shared = (struct data*) shmat(shmid, NULL, 0);

    shared->flag = 0;

    while(1){
        printf("Enter message: ");
        scanf(" %[^\n]", shared->text);

        shared->flag = 1;
    }

    shmdt(shared);

    return 0;
}