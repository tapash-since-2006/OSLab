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

    // 1. Create the shared memory block
    shmid = shmget(1234, sizeof(struct shared_data), 0666 | IPC_CREAT);
    if(shmid < 0) { perror("shmget failed"); exit(1); }

    sh = (struct shared_data*) shmat(shmid, NULL, 0);
    if(sh == (void*) -1) { perror("shmat failed"); exit(1); }

    // 2. Initialize variables and semaphores ONLY ONCE in the producer
    sh->in = 0;
    sh->out = 0;
    sem_init(&sh->mutex, 1, 1);
    sem_init(&sh->empty, 1, SIZE);
    sem_init(&sh->full, 1, 0);

    printf("Producer started. Initialized shared memory.\n");

    // 3. Produce 10 items
    for(int i=0; i<10; i++) {
        sem_wait(&sh->empty);
        sem_wait(&sh->mutex);

        sh->buffer[sh->in] = i;
        printf("Produced: %d\n", i);
        sh->in = (sh->in + 1) % SIZE;

        sem_post(&sh->mutex);
        sem_post(&sh->full);

        sleep(1);
    }

    printf("Producer finished producing. Exiting.\n");
    
    // 4. Detach, but DO NOT destroy. The consumer still needs the memory!
    shmdt(sh);

    return 0;
}



