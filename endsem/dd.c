#include <stdio.h>
#include <stdbool.h>

#define MAX_P 10

int n;
int available;
int allocation[MAX_P];
int request[MAX_P];

bool detectDeadlock() {
    int work = available;
    bool finish[MAX_P];

    // If no allocation → already finished
    for (int i = 0; i < n; i++) {
        if (allocation[i] == 0)
            finish[i] = true;
        else
            finish[i] = false;
    }

    bool found = true;
    while (found) {
        found = false;

        for (int i = 0; i < n; i++) {
            if (!finish[i] && request[i] <= work) {
                work += allocation[i];
                finish[i] = true;
                found = true;
            }
        }
    }

    bool deadlock = false;
    printf("\nDeadlocked processes: ");
    for (int i = 0; i < n; i++) {
        if (!finish[i]) {
            printf("P%d ", i);
            deadlock = true;
        }
    }

    if (!deadlock) printf("None");
    printf("\n");

    return deadlock;
}

int main() {
    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter available resource: ");
    scanf("%d", &available);

    printf("Enter Allocation:\n");
    for (int i = 0; i < n; i++) {
        printf("P%d: ", i);
        scanf("%d", &allocation[i]);
    }

    printf("Enter Request:\n");
    for (int i = 0; i < n; i++) {
        printf("P%d: ", i);
        scanf("%d", &request[i]);
    }

    printf("\n(a) Initial State:\n");
    if (detectDeadlock())
        printf("System is NOT SAFE (Deadlock exists)\n");
    else
        printf("System is SAFE\n");

    printf("\n(b) Additional Request\n");

    int pid, temp;
    printf("Enter process number: ");
    scanf("%d", &pid);

    printf("Enter additional request: ");
    scanf("%d", &temp);

    request[pid] += temp;

    if (detectDeadlock())
        printf("System is NOT SAFE (Deadlock occurs)\n");
    else
        printf("System is SAFE\n");

    return 0;
}