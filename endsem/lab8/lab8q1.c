#include <stdio.h>
#include <stdbool.h>

#define MAX_P 10
#define MAX_R 10

int n, m;
int avail[MAX_R];
int maxDemand[MAX_P][MAX_R];
int alloc[MAX_P][MAX_R];
int need[MAX_P][MAX_R];

void calculateNeed() {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            need[i][j] = maxDemand[i][j] - alloc[i][j];
}

void printState() {
    printf("\n%-5s %-15s %-15s %-15s\n", "Proc", "Allocation", "Max", "Need");
    printf("─────────────────────────────────────────────────\n");
    for (int i = 0; i < n; i++) {
        printf("P%-4d ", i);
        for (int j = 0; j < m; j++) printf("%d ", alloc[i][j]);
        printf("      ");
        for (int j = 0; j < m; j++) printf("%d ", maxDemand[i][j]);
        printf("      ");
        for (int j = 0; j < m; j++) printf("%d ", need[i][j]);
        printf("\n");
    }
    printf("Available: ");
    for (int j = 0; j < m; j++) printf("%d ", avail[j]);
    printf("\n");
}

bool isSafe(int safeSeq[]) {
    int work[MAX_R];
    bool finish[MAX_P] = {false};

    for (int j = 0; j < m; j++)
        work[j] = avail[j];

    int count = 0;
    while (count < n) {
        bool found = false;
        for (int i = 0; i < n; i++) {
            if (!finish[i]) {
                bool canAllocate = true;
                for (int j = 0; j < m; j++) {
                    if (need[i][j] > work[j]) {
                        canAllocate = false;
                        break;
                    }
                }
                if (canAllocate) {
                    for (int j = 0; j < m; j++)
                        work[j] += alloc[i][j];
                    finish[i] = true;
                    safeSeq[count++] = i;
                    found = true;
                }
            }
        }
        if (!found) return false;
    }
    return true;
}

bool requestResource(int pid, int request[]) {
    printf("\n>>> P%d requesting: ", pid);
    for (int j = 0; j < m; j++) printf("%d ", request[j]);
    printf("\n");

    for (int j = 0; j < m; j++) {
        if (request[j] > need[pid][j]) {
            printf("DENIED: Exceeds maximum claim. Need[P%d][%d]=%d but requested %d\n",
                   pid, j, need[pid][j], request[j]);
            return false;
        }
    }

    for (int j = 0; j < m; j++) {
        if (request[j] > avail[j]) {
            printf("WAIT: Resource %d not available. Available=%d, Requested=%d\n",
                   j, avail[j], request[j]);
            return false;
        }
    }

    for (int j = 0; j < m; j++) {
        avail[j]      -= request[j];
        alloc[pid][j] += request[j];
        need[pid][j]  -= request[j];
    }

    int safeSeq[MAX_P];
    if (isSafe(safeSeq)) {
        printf("GRANTED. Safe sequence: ");
        for (int i = 0; i < n; i++)
            printf("P%d%s", safeSeq[i], i < n-1 ? " -> " : "\n");
        printState();
        return true;
    } else {
        for (int j = 0; j < m; j++) {
            avail[j]      += request[j];
            alloc[pid][j] -= request[j];
            need[pid][j]  += request[j];
        }
        printf("DENIED: Leads to unsafe state. Rolled back.\n");
        return false;
    }
}

int main() {
    printf("Enter number of processes: ");
    scanf("%d", &n);
    printf("Enter number of resource types: ");
    scanf("%d", &m);

    printf("Enter Allocation matrix (%d x %d):\n", n, m);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            scanf("%d", &alloc[i][j]);

    printf("Enter Max matrix (%d x %d):\n", n, m);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            scanf("%d", &maxDemand[i][j]);

    printf("Enter Available resources (%d values): ", m);
    for (int j = 0; j < m; j++)
        scanf("%d", &avail[j]);

    calculateNeed();

    printf("\n══════════════════════════════════════\n");
    printf("(a) Need Matrix\n");
    printf("══════════════════════════════════════\n");
    for (int i = 0; i < n; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < m; j++)
            printf("%d ", need[i][j]);
        printf("\n");
    }

    printf("\n══════════════════════════════════════\n");
    printf("(b) Safety Check\n");
    printf("══════════════════════════════════════\n");
    int safeSeq[MAX_P];
    if (isSafe(safeSeq)) {
        printf("SAFE STATE. Sequence: ");
        for (int i = 0; i < n; i++)
            printf("P%d%s", safeSeq[i], i < n-1 ? " -> " : "\n");
    } else {
        printf("UNSAFE STATE!\n");
    }

    int choice;
    do {
        printf("\n══════════════════════════════════════\n");
        printf("Enter process number (0 to %d): ", n-1);
        int pid;
        scanf("%d", &pid);

        int request[MAX_R];
        printf("Enter request vector (%d values): ", m);
        for (int j = 0; j < m; j++)
            scanf("%d", &request[j]);

        requestResource(pid, request);

        printf("Another request? (1=Yes 0=No): ");
        scanf("%d", &choice);
    } while (choice == 1);

    return 0;
}