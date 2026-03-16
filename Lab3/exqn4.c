#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();
    int status;

    if (pid == 0) {
        // ----- Child -----
        time_t t = time(NULL);
        struct tm *tm_info = localtime(&t);

        int minute = tm_info->tm_min;
        printf("Child PID = %d, current minute = %d\n", getpid(), minute);

        if (minute % 2 == 1) {
            printf("Minute is odd → exiting with 0\n");
            exit(0);
        } else {
            printf("Minute is even → exiting with 1\n");
            exit(1);
        }
    } 
    else {
        // ----- Parent -----
        wait(&status);

        if (WIFEXITED(status)) {
            printf("Child exited with code = %d\n", WEXITSTATUS(status));
        } else {
            printf("Child did not exit normally\n");
        }
    }

    return 0;
}
