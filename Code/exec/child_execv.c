#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[]){
  printf("child_execv running \n");
  if(argc>1){
    printf("Argument received: %s\n", argv[1]);
  }
  return 42;
}