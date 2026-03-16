#include <stdio.h>
#include <pthread.h>

void* factorial(void *param)
{
    int n = (int)param;
    long fact = 1;

    for(int i = 1; i <= n; i++)
        fact *= i;

    printf("Thread for %d computed factorial = %ld\n", n, fact);

    return (void*)fact;
}

int main()
{
    pthread_t threads[5];
    int nums[5] = {3, 4, 5, 6, 7};
    long results[5];

    // create threads
    for(int i = 0; i < 5; i++)
        pthread_create(&threads[i], NULL, factorial, (void*)nums[i]);

    // wait and collect results
    for(int i = 0; i < 5; i++)
    {
        pthread_join(threads[i], (void**)&results[i]);
        printf("Main received factorial = %ld\n", results[i]);
    }

    return 0;
}
