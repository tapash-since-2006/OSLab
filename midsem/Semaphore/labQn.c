#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 20

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

sem_t empty, full, mutex;

void* producer(void* arg)
{
    int item = 1;

    while(item <= 20)
    {
        sem_wait(&empty);
        sem_wait(&mutex);

        buffer[in] = item;
        printf("Produced %d\n", item);

        in = (in + 1) % BUFFER_SIZE;
        item++;

        sem_post(&mutex);
        sem_post(&full);

        sleep(1);
    }

    return NULL;
}

void* consumer(void* arg)
{
    int item;

    while(1)
    {
        sem_wait(&full);
        sem_wait(&mutex);

        item = buffer[out];
        printf("Consumed %d\n", item);

        out = (out + 1) % BUFFER_SIZE;

        sem_post(&mutex);
        sem_post(&empty);

        sleep(2);
    }

    return NULL;
}

int main()
{
    pthread_t p, c;

    sem_init(&empty,0,10);   // trick: limit producer lead to 10
    sem_init(&full,0,0);
    sem_init(&mutex,0,1);

    pthread_create(&p,NULL,producer,NULL);
    pthread_create(&c,NULL,consumer,NULL);

    pthread_join(p,NULL);
    pthread_join(c,NULL);

    return 0;
}