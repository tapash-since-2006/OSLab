#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int sharedData = 0;
int readCount = 0;

sem_t mutex;     // protects readCount
sem_t wrt;       // writer lock for sharedData
sem_t readTry;   // blocks readers when writer wants access

void* reader(void* arg)
{
    int id = *(int*)arg;

    sem_wait(&readTry);        // check if writers want to enter
    sem_wait(&mutex);

    readCount++;
    if (readCount == 1)
        sem_wait(&wrt);        // first reader blocks writers

    sem_post(&mutex);
    sem_post(&readTry);

    printf("Reader %d reading data: %d\n", id, sharedData);
    sleep(1);

    sem_wait(&mutex);

    readCount--;
    if (readCount == 0)
        sem_post(&wrt);        // last reader allows writers

    sem_post(&mutex);

    return NULL;
}

void* writer(void* arg)
{
    int id = *(int*)arg;

    sem_wait(&readTry);   // block new readers
    sem_wait(&wrt);       // exclusive access to shared data

    sharedData += id;
    printf("Writer %d wrote data: %d\n", id, sharedData);

    sleep(2);

    sem_post(&wrt);
    sem_post(&readTry);   // allow readers again

    return NULL;
}

int main()
{
    pthread_t r1, r2, r3, w1, w2;
    int rId1 = 1, rId2 = 2, rId3 = 3;
    int wId1 = 10, wId2 = 20;

    sem_init(&mutex, 0, 1);
    sem_init(&wrt, 0, 1);
    sem_init(&readTry, 0, 1);

    pthread_create(&r1, NULL, reader, &rId1);
    pthread_create(&w1, NULL, writer, &wId1);
    pthread_create(&r2, NULL, reader, &rId2);
    pthread_create(&w2, NULL, writer, &wId2);
    pthread_create(&r3, NULL, reader, &rId3);

    pthread_join(r1, NULL);
    pthread_join(r2, NULL);
    pthread_join(r3, NULL);
    pthread_join(w1, NULL);
    pthread_join(w2, NULL);

    sem_destroy(&mutex);
    sem_destroy(&wrt);
    sem_destroy(&readTry);

    return 0;
}