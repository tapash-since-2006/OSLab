#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>


// using child process
int main() {
    pid_t pid;

    // --- ls -l ---
    pid = fork();
    if (pid == 0) {
        execl("/bin/ls", "ls", "-l", NULL);
        // exec only returns if it fails
        printf("Failed to execute ls\n");
    }
    else {
        wait(NULL);
    }

    // --- cp: copy file1.txt to file2.txt ---
    pid = fork();
    if (pid == 0) {
        execl("/bin/cp", "cp", "file1.txt", "file2.txt", NULL);
        printf("Failed to execute cp\n");
    }
    else {
        wait(NULL);
    }

    // --- wc: count lines of file2.txt ---
    pid = fork();
    if (pid == 0) {
        execl("/usr/bin/wc", "wc", "-l", "file2.txt", NULL);
        printf("Failed to execute wc\n");
    }
    else {
        wait(NULL);
    }

    return 0;
}


// using thread and multithreading

// #include <stdio.h>
// #include <pthread.h>
// #include <unistd.h>
// #include <sys/wait.h>

// void* run_ls(void *arg) {
//     pid_t pid = fork();
//     if (pid == 0) {
//         execl("/bin/ls", "ls", "-l", NULL);
//         printf("Failed to execute ls\n");
//     } else {
//         wait(NULL);
//     }
//     pthread_exit(NULL);
// }

// void* run_cp(void *arg) {
//     pid_t pid = fork();
//     if (pid == 0) {
//         execl("/bin/cp", "cp", "file1.txt", "file2.txt", NULL);
//         printf("Failed to execute cp\n");
//     } else {
//         wait(NULL);
//     }
//     pthread_exit(NULL);
// }

// void* run_wc(void *arg) {
//     pid_t pid = fork();
//     if (pid == 0) {
//         execl("/usr/bin/wc", "wc", "-l", "file2.txt", NULL);
//         printf("Failed to execute wc\n");
//     } else {
//         wait(NULL);
//     }
//     pthread_exit(NULL);
// }

// int main() {
//     pthread_t t1, t2, t3;

//     pthread_create(&t1, NULL, run_ls, NULL);
//     pthread_create(&t2, NULL, run_cp, NULL);
//     pthread_create(&t3, NULL, run_wc, NULL);

//     pthread_join(t1, NULL);
//     pthread_join(t2, NULL);
//     pthread_join(t3, NULL);

//     return 0;
// }
