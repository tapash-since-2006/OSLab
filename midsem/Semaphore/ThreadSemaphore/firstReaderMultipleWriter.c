#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Shared variables
int shared_data = 100;
int read_count = 0; // Keeps track of how many readers are actively reading

// Semaphores
sem_t resource; // Controls access to the shared data (used by writers and the first/last reader)
sem_t rmutex;   // Protects the read_count variable so readers don't overwrite each other

void *writer(void *arg) {
    int id = *((int *)arg);

    // 1. Writer requests exclusive access to the resource
    // If a reader is reading, or another writer is writing, it blocks here.
    sem_wait(&resource);
    
    // --- CRITICAL SECTION (WRITE) ---
    shared_data += 10;
    printf("Writer %d modified data to %d\n", id, shared_data);
    sleep(1); // Simulate time taken to write
    // --------------------------------

    // 2. Writer releases the resource
    sem_post(&resource);

    return NULL;
}

void *reader(void *arg) {
    int id = *((int *)arg);

    // 1. Reader registers itself safely
    sem_wait(&rmutex);
    read_count++;
    if (read_count == 1) {
        // If I am the VERY FIRST reader, I must lock the resource from writers.
        // If a writer is currently writing, I will block here.
        sem_wait(&resource); 
    }
    sem_post(&rmutex); // Release the counter lock so other readers can register

    // --- CRITICAL SECTION (READ) ---
    // Notice there is NO lock here stopping other readers!
    // Multiple readers can be in this section at the exact same time.
    printf("Reader %d read data as %d\n", id, shared_data);
    sleep(1); // Simulate time taken to read
    // -------------------------------

    // 2. Reader unregisters itself safely
    sem_wait(&rmutex);
    read_count--;
    if (read_count == 0) {
        // If I am the VERY LAST reader leaving, I unlock the resource 
        // so waiting writers can finally get a turn.
        sem_post(&resource); 
    }
    sem_post(&rmutex);

    return NULL;
}

int main() {
    pthread_t r[5], w[3]; // 5 readers, 3 writers
    int ids[5] = {1, 2, 3, 4, 5};

    // Initialize semaphores
    sem_init(&resource, 0, 1);
    sem_init(&rmutex, 0, 1);

    // Create a mix of reader and writer threads
    for (int i = 0; i < 3; i++) {
        pthread_create(&r[i], NULL, reader, &ids[i]);
    }
    for (int i = 0; i < 3; i++) {
        pthread_create(&w[i], NULL, writer, &ids[i]);
    }
    for (int i = 3; i < 5; i++) {
        pthread_create(&r[i], NULL, reader, &ids[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < 5; i++) {
        pthread_join(r[i], NULL);
    }
    for (int i = 0; i < 3; i++) {
        pthread_join(w[i], NULL);
    }

    // Destroy semaphores
    sem_destroy(&resource);
    sem_destroy(&rmutex);

    return 0;
}