#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();
    int status;

    if (pid == 0) {
        // ----- Child -----
        printf("Child started. PID = %d\n", getpid());
        printf("Parent PID before sleep = %d\n", getppid());

        sleep(3);  // simulate some work

        printf("Child finished work. PID = %d\n", getpid());
        exit(42);  // child exits with code 42
    }
    else {
        // ----- Parent -----
        printf("Parent PID = %d, waiting for child to finish...\n", getpid());

        wait(&status);  // wait for child

        if (WIFEXITED(status)) {
            int exit_code = WEXITSTATUS(status);
            printf("Child exited normally with return code = %d\n", exit_code);
        }
        else {
            printf("Child did not exit normally\n");
        }
    }

    return 0;
}
