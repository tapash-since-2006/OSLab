#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <semaphore.h>

#define SHM_KEY 1234

typedef struct {
    int data;
    int readcount;
    sem_t mutex;
    sem_t wrt;
} Shared;

int main() {
    int shmid = shmget(SHM_KEY, sizeof(Shared), 0666 | IPC_CREAT);
    Shared *sh = (Shared*)shmat(shmid, NULL, 0);

    // Initialize shared memory
    sh->data = 0;
    sh->readcount = 0;

    sem_init(&sh->mutex, 1, 1);
    sem_init(&sh->wrt, 1, 1);

    int r, w;
    printf("Enter number of readers: ");
    scanf("%d", &r);

    printf("Enter number of writers: ");
    scanf("%d", &w);

    // Table output
    printf("\n---------------------------\n");
    printf("Process Type   Count\n");
    printf("---------------------------\n");
    printf("Readers        %d\n", r);
    printf("Writers        %d\n", w);
    printf("---------------------------\n\n");

    // Create readers
    for(int i = 0; i < r; i++) {
        if(fork() == 0) {
            char *args[] = {"./reader", NULL};
            execv(args[0], args);
            perror("execv failed");
            exit(1);
        }
    }

    // Create writers
    for(int i = 0; i < w; i++) {
        if(fork() == 0) {
            char *args[] = {"./writer", NULL};
            execv(args[0], args);
            perror("execv failed");
            exit(1);
        }
    }

    // Wait for all children
    for(int i = 0; i < r + w; i++) {
        wait(NULL);
    }

    // Cleanup
    sem_destroy(&sh->mutex);
    sem_destroy(&sh->wrt);

    shmdt(sh);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}