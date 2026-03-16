#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        // ---- Child process ----
        printf("Child: loading previous program using exec\n");

        execl("./qn1", "qn1", NULL);

        // this line runs only if exec fails
        printf("Exec finished\n");
    }
    else {
        // ---- Parent process ----
        printf("Parent: waiting for child to complete\n");
        wait(NULL);
        printf("Parent: child completed\n");
    }

    return 0;
}
