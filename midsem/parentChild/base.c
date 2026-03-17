#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main(){
  int n,i;

  printf("Enter the number of elements\n");
  scanf("%d", &n);

  int arr[n];

  printf("Enter the elements:\n");
  for(int i=0;i<n;i++){
    scanf("%d", &arr[i]);
  }

    pid_t pid=fork();

    if(pid==0){
      printf("Child program peforming the sorting thing");

      char *args[n+2];
      // char **args=(char**)malloc((n+2)*sizeof(char*));
      args[0]="./first";

      for(int i=0;i<n;i++){
        args[i+1]=malloc(10);
        sprintf(args[i+1], "%d", arr[i]); // stores the value of arr[i] in args[i] after converting it to a string
      }

      args[n+1]=NULL;

      execv(args[0], args);
    }else{
      printf("Parent process is executing");
    }

    return 0;
}