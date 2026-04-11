#include <stdio.h>
#include <stdbool.h>

#define MAX_P 10
#define MAX_R 10

int n, m;
int available[MAX_R];
int allocation[MAX_P][MAX_R];
int request[MAX_P][MAX_R];

bool detectDeadlock() {
    int work[MAX_R];
    bool finish[MAX_P];

    for (int j = 0; j < m; j++)
        work[j] = available[j];

    for (int i = 0; i < n; i++) {
        bool hasAllocation = false;
        for (int j = 0; j < m; j++) {
            if (allocation[i][j] != 0) {
                hasAllocation = true;
                break;
            }
        }
        finish[i] = !hasAllocation;
    }

    bool found = true;
    while (found) {
        found = false;
        for (int i = 0; i < n; i++) {
            if (!finish[i]) {
                bool canProceed = true;

                for (int j = 0; j < m; j++) {
                    if (request[i][j] > work[j]) {
                        canProceed = false;
                        break;
                    }
                }

                if (canProceed) {
                    for (int j = 0; j < m; j++)
                        work[j] += allocation[i][j];

                    finish[i] = true;
                    found = true;
                }
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

    printf("Enter number of resource types: ");
    scanf("%d", &m);

    printf("Enter Available resources:\n");
    for (int j = 0; j < m; j++)
        scanf("%d", &available[j]);

    printf("Enter Allocation matrix (%d x %d):\n", n, m);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            scanf("%d", &allocation[i][j]);

    printf("Enter Request matrix (%d x %d):\n", n, m);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            scanf("%d", &request[i][j]);

    // (a) Initial check
    printf("\n(a) Initial State:\n");
    if (detectDeadlock())
        printf("System is NOT SAFE (Deadlock exists)\n");
    else
        printf("System is SAFE (No deadlock)\n");

// (b) User-defined additional request
printf("\n(b) Additional Request\n");

int pid, res, amount;

printf("Enter process number: ");
scanf("%d", &pid);

printf("Enter resource index (0 to %d): ", m - 1);
scanf("%d", &res);

printf("Enter number of instances to request: ");
scanf("%d", &amount);

// Apply the request
request[pid][res] += amount;

printf("\nAfter applying request:\n");

if (detectDeadlock())
    printf("System is NOT SAFE (Deadlock occurs)\n");
else
    printf("System is SAFE\n");

    return 0;
}