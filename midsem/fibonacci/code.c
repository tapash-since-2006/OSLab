#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Global array of size 2 initialized to all zeros
long long fib_arr[2] = {0, 0};

// Mutex permission variable to manage access
pthread_mutex_t permission;

// Global counter to handle the initial injection of '1' into the series
int term_count = 0;

void* generate_fibonacci(void* arg) {
    while (1) {
        // Take permission (Lock the mutex)
        pthread_mutex_lock(&permission);

        long long next_fib;

        // Compute the next element
        if (term_count == 0) {
            next_fib = 0;
            // Array stays {0, 0}
        } else if (term_count == 1) {
            next_fib = 1;
            // Update array so next iteration (0 + 1) works
            fib_arr[0] = 0;
            fib_arr[1] = 1;
        } else {
            // Add the two array elements to get the next element
            next_fib = fib_arr[0] + fib_arr[1];
            
            // Update the array so the next thread can compute the next element
            fib_arr[0] = fib_arr[1];
            fib_arr[1] = next_fib;
        }

        // Check if the fib element number is 40,000,000 or greater
        if (next_fib >= 40000000) {
            // Release permission before breaking out of the loop to prevent deadlock
            pthread_mutex_unlock(&permission);
            break;
        }

        // Print the next element along with the thread id
        printf("Thread %lu: %lld\n", (unsigned long)pthread_self(), next_fib);
        
        term_count++;

        // Release permission (Unlock the mutex)
        pthread_mutex_unlock(&permission);
    }
    
    return NULL;
}

int main() {
    pthread_t threads[4];

    // Initialize the mutex dynamically inside main
    pthread_mutex_init(&permission, NULL);

    // Create 4 threads without error checking
    for (int i = 0; i < 4; i++) {
        pthread_create(&threads[i], NULL, generate_fibonacci, NULL);
    }

    // Wait for all 4 threads to finish execution without error checking
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the mutex
    pthread_mutex_destroy(&permission);

    return 0;
}