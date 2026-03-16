#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>

int main(){
  int status;
  pid_t pid;

  pid=fork();

  if(pid==0){
    // it is the child process
    printf("I am the child Process and I exited with status 69\n");
    exit(69);
  }else{

    wait(&status);
    printf("I am the parent process and I waited for the child process to end\n");

    if(WIFEXITED(status)){
      printf("Child exited normally with the code %d\n", WEXITSTATUS(status));
    }else if (WIFSIGNALED(status)){
      printf("Child killed by the signal%d\n", WTERMSIG(status));
    }
  }

  return 0;
}