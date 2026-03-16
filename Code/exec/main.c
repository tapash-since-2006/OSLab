#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // fork(), exec*
#include <sys/types.h>  // pid_t
// #include <sys/wait.h>

int main() {
    int status;
    pid_t pid;

    // -------- execv example --------
    pid = fork();
    if(pid == 0) {
        printf("Child before execv\n");
        char *args[] = {"./child_execv", "hello", NULL};
        execv("./child_execv", args);
        perror("execv failed");  // If exec fails
        exit(1);
    } else {
        wait(&status);
        if(WIFEXITED(status))
            printf("Parent: child_execv exited with %d\n", WEXITSTATUS(status));
    }

    // -------- execl example --------
    pid = fork();
    if(pid == 0) {
        printf("Child before execl\n");
        execl("./child_execl", "child_execl", NULL);
        perror("execl failed");
        exit(1);
    } else {
        wait(&status);
        if(WIFEXITED(status))
            printf("Parent: child_execl exited with %d\n", WEXITSTATUS(status));
    }

    // -------- execlp example --------
    pid = fork();
    if(pid == 0) {
        printf("Child before execlp\n");
        execlp("./child_execlp", "child_execlp", NULL); // searches PATH
        perror("execlp failed");
        exit(1);
    } else {
        wait(&status);
        if(WIFEXITED(status))
            printf("Parent: child_execlp exited with %d\n", WEXITSTATUS(status));
    }

    // -------- execle example --------
    pid = fork();
    if(pid == 0) {
        printf("Child before execle\n");
        char *envp[] = {"MYVAR=HelloWorld", NULL};
        execle("./child_execle", "child_execle", NULL, envp);
        perror("execle failed");
        exit(1);
    } else {
        wait(&status);
        if(WIFEXITED(status))
            printf("Parent: child_execle exited with %d\n", WEXITSTATUS(status));
    }

    return 0;
}
