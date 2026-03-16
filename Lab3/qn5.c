#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int *fib;   // shared array
int n;      // number of terms

void* generateFib(void *arg) {

    if (n >= 1) fib[0] = 0;
    if (n >= 2) fib[1] = 1;

    for (int i = 2; i < n; i++) {
        fib[i] = fib[i-1] + fib[i-2];
    }

    printf("Child thread finished generating Fibonacci\n");
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

    n = atoi(argv[1]);

    fib = malloc(n * sizeof(int));

    pthread_t tid;

    // create child thread
    pthread_create(&tid, NULL, generateFib, NULL);

    // parent waits for child
    pthread_join(tid, NULL);

    // parent prints result
    printf("Fibonacci series:\n");
    for (int i = 0; i < n; i++)
        printf("%d ", fib[i]);

    printf("\n");

    return 0;
}
