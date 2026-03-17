// #include<stdio.h>
// #include<stdlib.h>
// #include<sys/shm.h>
// #include<unistd.h>

// struct data{
//   char letter;
//   int flag;
// };


// int main(){
//   int shmid;
//   struct data* shared;

//   shmid=shmget(1234, sizeof(struct data), 0666|IPC_CREAT);
//   shared=(struct data*) shmat(shmid, NULL,0);

//   shared->flag=0;
  
//   int pid=fork();

//   if(pid==0){
//     while(1){
//       if(shared->flag==1){
//         char ch=shared->letter;
//         ch=ch+1;
//         shared->letter=ch;
//         sleep(3);
//         shared->flag=2;
//       }
//     }
//   }else{
//     char input;
//     while(1){
//       printf("Enter the alphabet:");
//       scanf(" %c", &input);

//       shared->letter=input;
//       shared->flag=1;

//       while(shared->flag!=2);

//       printf("Next alphabet from the child: %c \n", shared->letter);

//       shared->flag=0;
//     }

//     shmdt(shared); // detaching
//     shmctl(shmid, IPC_RMID, NULL); //deleting the shared memory
//     return 0;
//   }
// }


#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include <semaphore.h> // Required for semaphores

struct data {
    char letter;
    sem_t sem_parent; // Parent waits here for the result
    sem_t sem_child;  // Child waits here for the input
};

int main() {
    int shmid;
    struct data* shared;

    // 1. Setup Shared Memory
    shmid = shmget(1234, sizeof(struct data), 0666 | IPC_CREAT);
    shared = (struct data*)shmat(shmid, NULL, 0);

    // 2. Initialize Semaphores
    // sem_init(pointer, pshared, initial_value)
    // pshared = 1 means it is shared between processes
    sem_init(&shared->sem_parent, 1, 0); // Start locked (0)
    sem_init(&shared->sem_child, 1, 0);  // Start locked (0)

    int pid = fork();

    if (pid == 0) { // CHILD PROCESS
        while (1) {
            sem_wait(&shared->sem_child); // Wait for parent to signal
            
            shared->letter = shared->letter + 1;
            printf("[Child] Processed to: %c\n", shared->letter);
            sleep(1); 
            
            sem_post(&shared->sem_parent); // Signal parent that result is ready
        }
    } 
    else { // PARENT PROCESS
        char input;
        while (1) {
            printf("Enter an alphabet: ");
            scanf(" %c", &input);

            shared->letter = input;
            
            sem_post(&shared->sem_child);  // Wake up the child
            
            sem_wait(&shared->sem_parent); // Sleep until child is done

            printf("[Parent] Next alphabet from child: %c\n", shared->letter);
        }

        // Cleanup
        sem_destroy(&shared->sem_parent);
        sem_destroy(&shared->sem_child);
        shmdt(shared);
        shmctl(shmid, IPC_RMID, NULL);
    }

    return 0;
}