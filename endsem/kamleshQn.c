#include <stdio.h>

#define MAX 100

int n, m;
int blocks[MAX];
int maxNeed[MAX];
int allocated[MAX];

int safeSeq[MAX * MAX]; // running history of granted requests
int safeCount = 0;

void sortBlocks() {
    for (int i = 0; i < m - 1; i++)
        for (int j = 0; j < m - i - 1; j++)
            if (blocks[j] < blocks[j + 1]) {
                int temp = blocks[j];
                blocks[j] = blocks[j + 1];
                blocks[j + 1] = temp;
            }
}

void printBlocks() {
    for (int i = 0; i < m; i++)
        printf("%d ", blocks[i]);
    printf("\n");
}

// Best Fit: array sorted descending, scan from smallest end
int bestFit(int req) {
    for (int i = m - 1; i >= 0; i--)
        if (blocks[i] >= req)
            return i;
    return -1;
}

// Print current safe state (cumulative granted sequence)
void checkSafety() {
    printf("System is SAFE\nSafe sequence: ");
    for (int i = 0; i < safeCount; i++)
        printf("P%d ", safeSeq[i]);
    printf("\n");
}

void processRequest(int pid, int req) {
    printf("\nP%d request %d bytes\n", pid, req);

    // Step 1: Sort & display before
    sortBlocks();
    printf("Memory before allocation (sorted):\n");
    printBlocks();

    // Step 2: Best Fit
    int idx = bestFit(req);
    if (idx == -1) {
        printf("-> Allocation failed (no suitable block)\n");
        return;
    }

    // Allocate
    blocks[idx] -= req;
    allocated[pid] += req;
    safeSeq[safeCount++] = pid; // append to running sequence

    printf("-> P%d allocated (remaining %d)\n", pid, blocks[idx]);

    // Step 3: Sort & display after
    sortBlocks();
    printf("Memory after allocation (sorted):\n");
    printBlocks();

    // Step 4: Safety check
    checkSafety();
}

void printFinal() {
    printf("\nFinal Allocation:\n");
    for (int i = 0; i < n; i++)
        printf("P%d: %d\n", i, allocated[i]);
}

int main() {
    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter number of memory blocks: ");
    scanf("%d", &m);

    printf("Enter sizes of memory blocks:\n");
    for (int i = 0; i < m; i++)
        scanf("%d", &blocks[i]);

    printf("Enter maximum memory each process can need:\n");
    for (int i = 0; i < n; i++) {
        printf("P%d: ", i);
        scanf("%d", &maxNeed[i]);
        allocated[i] = 0;
    }

    printf("\nEnter (pid request), -1 to stop:\n");

    while (1) {
        int pid;
        scanf("%d", &pid);
        if (pid == -1) break;

        int req;
        scanf("%d", &req);

        if (allocated[pid] + req > maxNeed[pid]) {
            printf("Request exceeds maximum need of P%d\n", pid);
            continue;
        }

        processRequest(pid, req);
    }

    printFinal();
    return 0;
}