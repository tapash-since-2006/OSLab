#include <stdio.h>
#include <stdbool.h>

#define MAX 100

int n, m;
int blocks[MAX];        // memory blocks
int maxNeed[MAX];
int alloc[MAX];
int need[MAX];

// ---------- SORT (DESCENDING) ----------
void sortBlocks() {
    for (int i = 0; i < m - 1; i++)
        for (int j = 0; j < m - i - 1; j++)
            if (blocks[j] < blocks[j + 1]) {
                int temp = blocks[j];
                blocks[j] = blocks[j + 1];
                blocks[j + 1] = temp;
            }
}

// ---------- PRINT BLOCKS ----------
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

// ---------- CALCULATE NEED ----------
void calculateNeed() {
    for (int i = 0; i < n; i++)
        need[i] = maxNeed[i] - alloc[i];
}

// ---------- SAFETY CHECK ----------
bool isSafe(int safeSeq[]) {
    int work = 0;
    bool finish[MAX] = {false};

    // Available = sum of blocks
    for (int i = 0; i < m; i++)
        work += blocks[i];

    int count = 0;

    while (count < n) {
        bool found = false;

        for (int i = 0; i < n; i++) {
            if (!finish[i] && need[i] <= work) {
                work += alloc[i];
                finish[i] = true;
                safeSeq[count++] = i;
                found = true;
            }
        }

        if (!found)
            return false;
    }

    return true;
}

// ---------- PROCESS REQUEST ----------
void processRequest(int pid, int req) {
    printf("\nP%d request %d bytes\n", pid, req);

    // Sort before
    sortBlocks();
    printf("Memory before allocation (sorted)\n");
    printBlocks();

    int idx = bestFit(req);
    if (idx == -1) {
        printf("-> Allocation failed\n");
        return;
    }

    // TEMP allocation
    blocks[idx] -= req;
    alloc[pid] += req;
    calculateNeed();

    printf("-> P%d allocated (remaining %d)\n", pid, blocks[idx]);

    // Sort after
    sortBlocks();
    printf("Memory after allocation (sorted)\n");
    printBlocks();

    // SAFETY CHECK
    int safeSeq[MAX];
    if (isSafe(safeSeq)) {
        printf("System is SAFE\nSafe sequence ");
        for (int i = 0; i < n; i++)
            printf("P%d ", safeSeq[i]);
        printf("\n");
    } else {
        printf("System is UNSAFE! Rolling back\n");

        // rollback
        alloc[pid] -= req;
        blocks[idx] += req;
        calculateNeed();
    }
}

// ---------- FINAL ----------
void printFinal() {
    printf("\nFinal Allocation\n");
    for (int i = 0; i < n; i++)
        printf("P%d: %d\n", i, alloc[i]);
}

// ---------- MAIN ----------
int main() {
    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter number of memory blocks: ");
    scanf("%d", &m);

    printf("Enter block sizes:\n");
    for (int i = 0; i < m; i++)
        scanf("%d", &blocks[i]);

    printf("Enter max need:\n");
    for (int i = 0; i < n; i++) {
        printf("P%d: ", i);
        scanf("%d", &maxNeed[i]);
        alloc[i] = 0;
    }

    calculateNeed();

    printf("\nEnter (pid request), -1 to stop:\n");

    while (1) {
        int pid;
        scanf("%d", &pid);
        if (pid == -1) break;

        int req;
        scanf("%d", &req);

        if (alloc[pid] + req > maxNeed[pid]) {
            printf("Exceeds max need\n");
            continue;
        }

        processRequest(pid, req);
    }

    printFinal();
    return 0;
}