#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;

    pid = fork();   // create child process

    if (pid == 0) {
        // ----- Child Process -----
        printf("I am CHILD process\n");
        printf("Child PID  = %d\n", getpid());
        printf("Child PPID = %d\n", getppid());

        printf("Child is doing some work...\n");
        sleep(3);

        printf("Child finished\n");
    }
    else {
        // ----- Parent Process -----
        printf("I am PARENT process\n");
        printf("Parent PID  = %d\n", getpid());
        printf("Parent PPID = %d\n", getppid());

        // wait for child to finish
        wait(NULL);

        printf("Parent resumes after child completes\n");
    }

    return 0;
}
