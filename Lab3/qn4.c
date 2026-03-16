#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int n;

typedef struct {
    int *arr;
} Data;

/* ---------- Utility ---------- */
void printArray(int *a) {
    for (int i = 0; i < n; i++)
        printf("%d ", a[i]);
    printf("\n");
}

/* ---------- Bubble Sort ---------- */
void* bubbleSort(void *arg) {
    Data *d = (Data*)arg;
    int *a = d->arr;

    clock_t start = clock();

    for (int i = 0; i < n-1; i++)
        for (int j = 0; j < n-i-1; j++)
            if (a[j] > a[j+1]) {
                int t = a[j]; a[j] = a[j+1]; a[j+1] = t;
            }

    clock_t end = clock();
    printf("Bubble sort time = %.6f sec\n",
           (double)(end-start)/CLOCKS_PER_SEC);

    pthread_exit(a);
}

/* ---------- Insertion Sort ---------- */
void* insertionSort(void *arg) {
    Data *d = (Data*)arg;
    int *a = d->arr;

    clock_t start = clock();

    for (int i = 1; i < n; i++) {
        int key = a[i], j = i-1;
        while (j >= 0 && a[j] > key) {
            a[j+1] = a[j];
            j--;
        }
        a[j+1] = key;
    }

    clock_t end = clock();
    printf("Insertion sort time = %.6f sec\n",
           (double)(end-start)/CLOCKS_PER_SEC);

    pthread_exit(a);
}

/* ---------- Quick Sort ---------- */
int partition(int a[], int low, int high) {
    int pivot = a[high], i = low-1;
    for (int j = low; j < high; j++) {
        if (a[j] <= pivot) {
            i++;
            int t=a[i]; a[i]=a[j]; a[j]=t;
        }
    }
    int t=a[i+1]; a[i+1]=a[high]; a[high]=t;
    return i+1;
}

void quick(int a[], int low, int high) {
    if (low < high) {
        int p = partition(a, low, high);
        quick(a, low, p-1);
        quick(a, p+1, high);
    }
}

void* quickSort(void *arg) {
    Data *d = (Data*)arg;
    int *a = d->arr;

    clock_t start = clock();
    quick(a, 0, n-1);
    clock_t end = clock();

    printf("Quick sort time = %.6f sec\n",
           (double)(end-start)/CLOCKS_PER_SEC);

    pthread_exit(a);
}

/* ---------- Main ---------- */
int main(int argc, char *argv[]) {

    n = atoi(argv[1]);

    int *base = malloc(n*sizeof(int));
    for (int i = 0; i < n; i++)
        base[i] = atoi(argv[i+2]);

    // make copies for each thread
    int *a1 = malloc(n*sizeof(int));
    int *a2 = malloc(n*sizeof(int));
    int *a3 = malloc(n*sizeof(int));

    for (int i = 0; i < n; i++)
        a1[i] = a2[i] = a3[i] = base[i];

    Data d1={a1}, d2={a2}, d3={a3};

    pthread_t t1, t2, t3;

    pthread_create(&t1, NULL, bubbleSort, &d1);
    pthread_create(&t2, NULL, insertionSort, &d2);
    pthread_create(&t3, NULL, quickSort, &d3);

    int *sorted;

    pthread_join(t1, (void**)&sorted);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    printf("\nFinal sorted array (from bubble thread):\n");
    printArray(sorted);

    return 0;
}


// compile example gcc sort_threads.c -o sort_threads -pthread

// execution example ./sort_threads 6 9 3 7 1 5 2
