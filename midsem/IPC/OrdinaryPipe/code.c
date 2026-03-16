#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>


int main(){
    int fd[2];
    char buffer[100];
    pid_t pid;

    pipe(fd);

    pid=fork();

    if(pid==0){
        close(fd[1]);
        read(fd[0], buffer, sizeof(buffer));
        printf("Child received: %s \n", buffer);
        close(fd[0]);
    }else{
        char msg[]="Hello From the parent";
        close(fd[0]);
        write(fd[1], msg, strlen(msg)+1);
        close(fd[1]);
    }

    return 0;
}