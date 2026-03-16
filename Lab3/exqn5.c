// M-1 making A, B and C as global 
// hence no need of passing and returning the values
// simply pass the row and column number via struct and calcukate as per requiremnt

// M-1: Global matrices version
// #include <stdio.h>
// #include <stdlib.h>
// #include <pthread.h>

// #define MAX 10

// int A[MAX][MAX] = {{1, 2, 3}, {4, 5, 6}};
// int B[MAX][MAX] = {{7, 8}, {9, 10}, {11, 12}};
// int C[MAX][MAX];
// int n1 = 2, m1 = 3, n2 = 3, m2 = 2;

// typedef struct {
//     int row;
//     int col;
// } v;

// void* multiply(void* arg) {
//     v* data = (v*) arg;
//     int sum = 0;
//     for (int k = 0; k < m1; k++)
//         sum += A[data->row][k] * B[k][data->col];
//     C[data->row][data->col] = sum;
//     pthread_exit(0);
// }

// int main() {
//     pthread_t threads[MAX][MAX];
//     v args[MAX][MAX];

//     for (int i = 0; i < n1; i++)
//         for (int j = 0; j < m2; j++) {
//             args[i][j].row = i;
//             args[i][j].col = j;
//             pthread_create(&threads[i][j], NULL, multiply, (void*)&args[i][j]);
//         }

//     for (int i = 0; i < n1; i++)
//         for (int j = 0; j < m2; j++)
//             pthread_join(threads[i][j], NULL);

//     printf("Result Matrix C = A x B:\n");
//     for (int i = 0; i < n1; i++) {
//         for (int j = 0; j < m2; j++)
//             printf("%d ", C[i][j]);
//         printf("\n");
//     }

//     return 0;
// }

//M-2
// this one is without using global variable
// but we pass the required input and also 
// the output varibale in the struct while 
// giving the input hence possible

// M-2: Passing matrices in struct, no global arrays
// M-2: Passing matrices in struct, no global arrays
// #include <stdio.h>
// #include <stdlib.h>
// #include <pthread.h>

// #define MAX 10

// typedef struct {
//     int row;
//     int col;
//     int m1;
//     int (*A)[MAX];
//     int (*B)[MAX];
//     int (*C)[MAX];
// } v_ptr;

// // Thread function — must be outside main
// void* multiply(void* arg) {
//     v_ptr* data = (v_ptr*) arg;
//     int sum = 0;
//     for (int k = 0; k < data->m1; k++)
//         sum += data->A[data->row][k] * data->B[k][data->col];
//     data->C[data->row][data->col] = sum;
//     pthread_exit(NULL);
// }

// int main() {
//     int n1 = 2, m1 = 3, n2 = 3, m2 = 2;
//     int A[MAX][MAX] = {{1, 2, 3}, {4, 5, 6}};
//     int B[MAX][MAX] = {{7, 8}, {9, 10}, {11, 12}};
//     int C[MAX][MAX];

//     pthread_t threads[MAX][MAX];
//     v_ptr args[MAX][MAX];

//     // Create threads
//     for (int i = 0; i < n1; i++)
//         for (int j = 0; j < m2; j++) {
//             args[i][j].row = i;
//             args[i][j].col = j;
//             args[i][j].m1 = m1;
//             args[i][j].A = A;
//             args[i][j].B = B;
//             args[i][j].C = C;
//             pthread_create(&threads[i][j], NULL, multiply, (void*)&args[i][j]);
//         }

//     // Wait for all threads
//     for (int i = 0; i < n1; i++)
//         for (int j = 0; j < m2; j++)
//             pthread_join(threads[i][j], NULL);

//     printf("Result Matrix C = A x B:\n");
//     for (int i = 0; i < n1; i++) {
//         for (int j = 0; j < m2; j++)
//             printf("%d ", C[i][j]);
//         printf("\n");
//     }

//     return 0;
// }






//M-3
// same but returing value from the thread function instead of
// entering the ans array in the struct input

// M-3: Thread returns value like factorial example
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>

#define MAX 10

int A[MAX][MAX] = {{1, 2, 3}, {4, 5, 6}};
int B[MAX][MAX] = {{7, 8}, {9, 10}, {11, 12}};
int n1 = 2, m1 = 3, n2 = 3, m2 = 2;

typedef struct {
    int row;
    int col;
} v_ret;

void* multiply(void* arg) {
    v_ret* data = (v_ret*) arg;
    int sum = 0;
    for (int k = 0; k < m1; k++)
        sum += A[data->row][k] * B[k][data->col];
    pthread_exit((void*)(intptr_t)sum);
}

int main() {
    int C[MAX][MAX];
    pthread_t threads[MAX][MAX];
    v_ret args[MAX][MAX];

    for (int i = 0; i < n1; i++)
        for (int j = 0; j < m2; j++) {
            args[i][j].row = i;
            args[i][j].col = j;
            pthread_create(&threads[i][j], NULL, multiply, (void*)&args[i][j]);
        }

    for (int i = 0; i < n1; i++)
        for (int j = 0; j < m2; j++) {
            void* ret;
            pthread_join(threads[i][j], &ret);
            C[i][j] = (int)(intptr_t)ret;
        }

    printf("Result Matrix C = A x B:\n");
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < m2; j++)
            printf("%d ", C[i][j]);
        printf("\n");
    }

    return 0;
}
