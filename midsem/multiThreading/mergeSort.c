#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int arr[100];

typedef struct {
    int left;
    int right;
} Range;

void merge(int l, int m, int r) {
    int i = l, j = m + 1, k = 0;
    int temp[100];

    while(i <= m && j <= r) {
        if(arr[i] <= arr[j])
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }

    while(i <= m)
        temp[k++] = arr[i++];

    while(j <= r)
        temp[k++] = arr[j++];

    for(i = 0; i < k; i++)
        arr[l + i] = temp[i];
}

void *merge_sort(void *arg) {
    Range *range = (Range*) arg;
    
    int l = range->left;
    int r = range->right;

    // Base case: 1 or 0 elements
    if(l >= r) {
        return NULL;
    }

    int mid = (l + r) / 2;

    // Allocate safe memory for the child threads' arguments
    Range *left_range = malloc(sizeof(Range));
    left_range->left = l;
    left_range->right = mid;

    Range *right_range = malloc(sizeof(Range));
    right_range->left = mid + 1;
    right_range->right = r;

    pthread_t t1, t2;

    // Spawn a new thread for each half
    pthread_create(&t1, NULL, merge_sort, left_range);
    pthread_create(&t2, NULL, merge_sort, right_range);

    // Wait for both halves to finish sorting
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // Merge the two sorted halves
    merge(l, mid, r);

    return NULL;
}

int main() {
    int n;

    printf("Enter number of elements: ");
    scanf("%d", &n);

    printf("Enter elements:\n");
    for(int i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    // Setup the initial range for the root thread
    Range *full = (Range*)malloc(sizeof(Range));
    full->left = 0;
    full->right = n - 1;

    // Create the initial thread from main
    pthread_t root_thread;
    pthread_create(&root_thread, NULL, merge_sort, full);
    
    // Main thread waits here until the entire sorting process is complete
    pthread_join(root_thread, NULL);
    
    printf("Sorted array:\n");
    for(int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");

    return 0;
}