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

    printf("Inside Writer process with PID %d\n", getpid());

    sem_wait(&sh->wrt);

    // Critical section
    sh->data += 10;
    printf("Writer %d is writing data: %d\n", getpid(), sh->data);
    sleep(1);

    sem_post(&sh->wrt);

    shmdt(sh);
    return 0;
}