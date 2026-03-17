#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>

#define SIZE 5

struct shared_data {
    int buffer[SIZE];
    int in;
    int out;
    sem_t mutex;
    sem_t empty;
    sem_t full;
};

int main() {
    int shmid;
    struct shared_data *sh;

    // 1. Access the existing shared memory (No IPC_CREAT flag here)
    shmid = shmget(1234, sizeof(struct shared_data), 0666);
    if(shmid < 0) { perror("shmget failed. Did you run the producer first?"); exit(1); }

    sh = (struct shared_data*) shmat(shmid, NULL, 0);
    if(sh == (void*) -1) { perror("shmat failed"); exit(1); }

    printf("Consumer started. Attached to shared memory.\n");

    // 2. Consume 10 items
    for(int i=0; i<10; i++) {
        sem_wait(&sh->full);
        sem_wait(&sh->mutex);

        int item = sh->buffer[sh->out];
        printf("Consumed: %d\n", item);
        sh->out = (sh->out + 1) % SIZE;

        sem_post(&sh->mutex);
        sem_post(&sh->empty);

        sleep(2);
    }

    printf("Consumer finished. Cleaning up shared memory.\n");

    // 3. Clean up semaphores and destroy the shared memory block
    sem_destroy(&sh->mutex);
    sem_destroy(&sh->empty);
    sem_destroy(&sh->full);

    shmdt(sh);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}