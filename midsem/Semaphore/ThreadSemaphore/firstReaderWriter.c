#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int sharedData = 0;
int readCount = 0;
sem_t mutex, wrt; // mutex protects readCount, wrt protects sharedData

void* reader(void* arg) {
    int id = *(int*)arg;

    sem_wait(&mutex);
    readCount++;
    if(readCount == 1) sem_wait(&wrt); // first reader locks writer
    sem_post(&mutex);

    printf("Reader %d reading data: %d\n", id, sharedData);
    sleep(1); // simulate reading

    sem_wait(&mutex);
    readCount--;
    if(readCount == 0) sem_post(&wrt); // last reader unlocks writer
    sem_post(&mutex);

    return NULL;
}

void* writer(void* arg) {
    int id = *(int*)arg;

    sem_wait(&wrt);
    sharedData += id;
    printf("Writer %d wrote data: %d\n", id, sharedData);
    sleep(2); // simulate writing
    sem_post(&wrt);

    return NULL;
}

int main() {
    pthread_t r1, r2, w1;
    int id1 = 1, id2 = 2, idw = 10;

    sem_init(&mutex, 0, 1);
    sem_init(&wrt, 0, 1);

    pthread_create(&r1, NULL, reader, &id1);
    pthread_create(&r2, NULL, reader, &id2);
    pthread_create(&w1, NULL, writer, &idw);

    pthread_join(r1, NULL);
    pthread_join(r2, NULL);
    pthread_join(w1, NULL);

    sem_destroy(&mutex);
    sem_destroy(&wrt);

    return 0;
}