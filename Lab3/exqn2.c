#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        // ----- Child -----
        printf("Child started. PID = %d\n", getpid());
        printf("Parent PID before becoming orphan = %d\n", getppid());

        // sleep to allow parent to exit
        sleep(5);

        printf("Parent PID after becoming orphan = %d\n", getppid());
        printf("Child is now adopted by init (PID 1)\n");
    }
    else {
        // ----- Parent -----
        printf("Parent PID = %d, exiting now\n", getpid());
        exit(0);  // parent exits immediately
    }

    return 0;
}
