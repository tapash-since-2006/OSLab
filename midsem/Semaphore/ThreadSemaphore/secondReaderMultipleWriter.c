#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Shared variables
int shared_data = 100;
int read_count = 0;
int write_count = 0;

// Semaphores
sem_t resource;  // Controls access to the shared data
sem_t rmutex;    // Protects read_count
sem_t wmutex;    // Protects write_count
sem_t readTry;   // Used by writers to block readers

void *writer(void *arg) {
    int id = *((int *)arg);

    // 1. Writer declares intention to write
    sem_wait(&wmutex);
    write_count++;
    if (write_count == 1) {
        // If first writer, block readers from trying to enter
        sem_wait(&readTry); 
    }
    sem_post(&wmutex);

    // 2. Writer waits for the resource to be free of readers and other writers
    sem_wait(&resource);
    
    // --- CRITICAL SECTION (WRITE) ---
    shared_data += 10;
    printf("Writer %d modified data to %d\n", id, shared_data);
    sleep(1); // Simulate time taken to write
    // --------------------------------

    sem_post(&resource);

    // 3. Writer finishes
    sem_wait(&wmutex);
    write_count--;
    if (write_count == 0) {
        // If no more writers are waiting, let readers try again
        sem_post(&readTry); 
    }
    sem_post(&wmutex);

    return NULL;
}

void *reader(void *arg) {
    int id = *((int *)arg);

    // 1. Reader tries to enter (will be blocked here if a writer is waiting)
    sem_wait(&readTry);
    
    // 2. Reader registers itself
    sem_wait(&rmutex);
    read_count++;
    if (read_count == 1) {
        // If first reader, lock the resource from writers
        sem_wait(&resource); 
    }
    sem_post(&rmutex);
    
    // 3. Release readTry so other readers can enter (if no writer is waiting)
    sem_post(&readTry);

    // --- CRITICAL SECTION (READ) ---
    printf("Reader %d read data as %d\n", id, shared_data);
    sleep(1); // Simulate time taken to read
    // -------------------------------

    // 4. Reader finishes
    sem_wait(&rmutex);
    read_count--;
    if (read_count == 0) {
        // If last reader, release the resource for writers
        sem_post(&resource); 
    }
    sem_post(&rmutex);

    return NULL;
}

int main() {
    pthread_t r[5], w[5];
    int ids[5] = {1, 2, 3, 4, 5};

    // Initialize semaphores
    sem_init(&resource, 0, 1);
    sem_init(&rmutex, 0, 1);
    sem_init(&wmutex, 0, 1);
    sem_init(&readTry, 0, 1);

    // Create threads (mixing readers and writers to see the priority in action)
    for (int i = 0; i < 3; i++) {
        pthread_create(&r[i], NULL, reader, &ids[i]);
    }
    for (int i = 0; i < 2; i++) {
        pthread_create(&w[i], NULL, writer, &ids[i]);
    }
    for (int i = 3; i < 5; i++) {
        pthread_create(&r[i], NULL, reader, &ids[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < 5; i++) {
        pthread_join(r[i], NULL);
    }
    for (int i = 0; i < 2; i++) {
        pthread_join(w[i], NULL);
    }

    // Destroy semaphores
    sem_destroy(&resource);
    sem_destroy(&rmutex);
    sem_destroy(&wmutex);
    sem_destroy(&readTry);

    return 0;
}