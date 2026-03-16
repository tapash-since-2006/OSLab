#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

int A[10][10], B[10][10], C[10][10];
int m,n,p;

typedef struct{
  int row;
  int col;
}element;

void *multiply(void *args){
  element *data=(element*) args;

  int row=data->row;
  int col=data->col;

  C[row][col]=0;

  for(int i=0;i<n;i++){
    C[row][col]+=A[row][i]*B[i][col];
  }

  pthread_exit(0);
}

int main()
{
    printf("Enter rows and columns of matrix A: ");
    scanf("%d%d",&m,&n);

    printf("Enter columns of matrix B: ");
    scanf("%d",&p);

    printf("Enter matrix A:\n");
    for(int i=0;i<m;i++)
        for(int j=0;j<n;j++)
            scanf("%d",&A[i][j]);

    printf("Enter matrix B:\n");
    for(int i=0;i<n;i++)
        for(int j=0;j<p;j++)
            scanf("%d",&B[i][j]);

    pthread_t threads[m][p];

    for(int i=0;i<m;i++)
    {
        for(int j=0;j<p;j++)
        {
            element *data = malloc(sizeof(element));
            data->row = i;
            data->col = j;

            pthread_create(&threads[i][j],NULL,multiply,data);
        }
    }

    for(int i=0;i<m;i++)
    {
        for(int j=0;j<p;j++)
        {
            pthread_join(threads[i][j],NULL);
        }
    }

    printf("Result matrix:\n");

    for(int i=0;i<m;i++)
    {
        for(int j=0;j<p;j++)
        {
            printf("%d ",C[i][j]);
        }
        printf("\n");
    }

    return 0;
}



// #include <stdio.h>
// #include <pthread.h>

// int A[10][10],B[10][10],C[10][10];
// int r1,c1,r2,c2;

// void *multiply(void *arg){

//     for(int i=0;i<r1;i++)
//         for(int j=0;j<c2;j++){
//             C[i][j]=0;
//             for(int k=0;k<c1;k++)
//                 C[i][j]+=A[i][k]*B[k][j];
//         }

//     pthread_exit(NULL);
// }

// int main(){

//     pthread_t tid;

//     printf("Enter rows and columns of matrix A: ");
//     scanf("%d%d",&r1,&c1);

//     printf("Enter rows and columns of matrix B: ");
//     scanf("%d%d",&r2,&c2);

//     if(c1!=r2){
//         printf("Multiplication not possible\n");
//         return 0;
//     }

//     printf("Enter Matrix A:\n");
//     for(int i=0;i<r1;i++)
//         for(int j=0;j<c1;j++)
//             scanf("%d",&A[i][j]);

//     printf("Enter Matrix B:\n");
//     for(int i=0;i<r2;i++)
//         for(int j=0;j<c2;j++)
//             scanf("%d",&B[i][j]);

//     pthread_create(&tid,NULL,multiply,NULL);
//     pthread_join(tid,NULL);

//     printf("Result Matrix:\n");

//     for(int i=0;i<r1;i++){
//         for(int j=0;j<c2;j++)
//             printf("%d ",C[i][j]);
//         printf("\n");
//     }

//     return 0;
// }