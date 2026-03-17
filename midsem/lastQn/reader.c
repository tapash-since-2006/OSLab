#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <semaphore.h>

#define SHM_KEY 1234

typedef struct {
    int data;
    int readcount;
    sem_t mutex;
    sem_t wrt;
} Shared;

int main() {
    int shmid = shmget(SHM_KEY, sizeof(Shared), 0666);
    Shared *sh = (Shared*)shmat(shmid, NULL, 0);

    printf("Inside Reader process with PID %d\n", getpid());

    // Entry section
    sem_wait(&sh->mutex);
    sh->readcount++;

    if(sh->readcount == 1)
        sem_wait(&sh->wrt); // first reader locks writer

    sem_post(&sh->mutex);

    // Critical section
    printf("Reader %d is reading data: %d\n", getpid(), sh->data);
    sleep(1);

    // Exit section
    sem_wait(&sh->mutex);
    sh->readcount--;

    if(sh->readcount == 0)
        sem_post(&sh->wrt); // last reader unlocks writer

    sem_post(&sh->mutex);

    shmdt(sh);
    return 0;
}