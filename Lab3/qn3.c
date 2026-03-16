#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        // ---- Child ----
        printf("Child process: PID = %d exiting now\n", getpid());
        exit(0);
    }
    else {
        // ---- Parent ----
        printf("Parent process: PID = %d sleeping\n", getpid());
        sleep(30);   // keep parent alive without wait()
        printf("Parent exiting\n");
    }

    return 0;
}
