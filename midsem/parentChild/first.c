#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[]){
  int n=argc-1;
  int arr[n];
  int i, j, temp;

  for(i=0;i<n;i++){
    arr[i]=atoi(argv[i+1]);
  }
  for(i=0;i<n-1;i++)
    {
        for(j=0;j<n-i-1;j++)
        {
            if(arr[j] > arr[j+1])
            {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }

    printf("Sorted array:\n");

    for(i=0;i<n;i++)
        printf("%d ", arr[i]);

    printf("\n");

    return 0;
}