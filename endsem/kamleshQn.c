#include <stdio.h>

#define MAX 100

int n, m;
int blocks[MAX];
int maxNeed[MAX];
int allocated[MAX];

// ---------- SORT BLOCKS (DESCENDING) ----------
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

// ---------- BEST FIT ----------
int bestFit(int req) {
    for (int i = m - 1; i >= 0; i--)   // smallest suitable
        if (blocks[i] >= req)
            return i;
    return -1;
}

// ---------- BANKER'S SAFETY CHECK ----------
int isSafe() {
    int work = 0;
    int finish[MAX] = {0};
    int need[MAX];

    // Available = total free memory
    for (int i = 0; i < m; i++)
        work += blocks[i];

    // Need = Max - Allocated
    for (int i = 0; i < n; i++)
        need[i] = maxNeed[i] - allocated[i];

    int safeSeq[MAX];
    int count = 0;

    while (count < n) {
        int found = 0;

        for (int i = 0; i < n; i++) {
            if (!finish[i] && need[i] <= work) {
                work += allocated[i];
                safeSeq[count++] = i;
                finish[i] = 1;
                found = 1;
            }
        }

        if (!found)
            return 0;  // unsafe
    }

    // PRINT SAFE SEQUENCE
    printf("System is SAFE\nSafe sequence: ");
    for (int i = 0; i < n; i++)
        printf("P%d ", safeSeq[i]);
    printf("\n");

    return 1;
}

// ---------- PROCESS REQUEST ----------
void processRequest(int pid, int req) {
    printf("\nP%d request %d bytes\n", pid, req);

    // Sort before
    sortBlocks();
    printf("Memory before allocation (sorted):\n");
    printBlocks();

    // Best fit
    int idx = bestFit(req);
    if (idx == -1) {
        printf("-> Allocation failed (no suitable block)\n");
        return;
    }

    // Try allocation (temporary)
    blocks[idx] -= req;
    allocated[pid] += req;

    printf("-> P%d allocated (remaining %d)\n", pid, blocks[idx]);

    // Sort after
    sortBlocks();
    printf("Memory after allocation (sorted):\n");
    printBlocks();

    // Safety check
    if (!isSafe()) {
        printf("System is UNSAFE! Rolling back...\n");

        // rollback
        allocated[pid] -= req;
        blocks[idx] += req;
    }
}

// ---------- FINAL OUTPUT ----------
void printFinal() {
    printf("\nFinal Allocation:\n");
    for (int i = 0; i < n; i++)
        printf("P%d: %d\n", i, allocated[i]);
}

// ---------- MAIN ----------
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