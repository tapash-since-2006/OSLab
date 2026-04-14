#include <stdio.h>

#define MAX 100

int n, m;
int blocks[MAX];
int maxNeed[MAX];
int allocated[MAX];

// 🔹 Sort blocks in descending order
void sortBlocks() {
    for (int i = 0; i < m - 1; i++) {
        for (int j = 0; j < m - i - 1; j++) {
            if (blocks[j] < blocks[j + 1]) {
                int temp = blocks[j];
                blocks[j] = blocks[j + 1];
                blocks[j + 1] = temp;
            }
        }
    }
}

// 🔹 Print memory blocks
void printBlocks() {
    for (int i = 0; i < m; i++)
        printf("%d ", blocks[i]);
    printf("\n");
}

// 🔹 Best Fit Allocation (using sorted array)
int bestFit(int req) {
    for (int i = m - 1; i >= 0; i--) { // smallest first
        if (blocks[i] >= req) {
            return i; // FIRST match = best fit
        }
    }
    return -1;
}

// 🔹 Safety Check (simplified Banker)
void checkSafety() {
    int finish[MAX] = {0};
    int work = 0;

    // total free memory
    for (int i = 0; i < m; i++)
        work += blocks[i];

    int safeSeq[MAX];
    int count = 0;

    while (1) {
        int found = 0;

        for (int i = 0; i < n; i++) {
            if (!finish[i]) {
                int need = maxNeed[i] - allocated[i];

                if (need <= work) {
                    work += allocated[i];
                    safeSeq[count++] = i;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }

        if (!found) break;
    }

    int safe = 1;
    for (int i = 0; i < n; i++) {
        if (!finish[i]) {
            safe = 0;
            break;
        }
    }

    if (safe) {
        printf("System is SAFE\nSafe sequence: ");
        for (int i = 0; i < count; i++)
            printf("P%d ", safeSeq[i]);
        printf("\n");
    } else {
        printf("System is NOT SAFE\n");
    }
}

// 🔹 Process a single request
void processRequest(int pid, int req) {
    printf("\nP%d request %d bytes\n", pid, req);

    // Step 1: Sort & display
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

    printf("-> P%d allocated (remaining %d)\n", pid, blocks[idx]);

    // Step 3: Sort again
    sortBlocks();
    printf("Memory after allocation (sorted):\n");
    printBlocks();

    // Step 4: Safety check
    checkSafety();
}

// 🔹 Final allocation display
void printFinal() {
    printf("\nFinal Allocation:\n");
    for (int i = 0; i < n; i++) {
        printf("P%d: %d\n", i, allocated[i]);
    }
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
        int pid, req;
        scanf("%d", &pid);

        if (pid == -1) break;

        scanf("%d", &req);

        // Optional max check
        if (allocated[pid] + req > maxNeed[pid]) {
            printf("Request exceeds maximum need of P%d\n", pid);
            continue;
        }

        processRequest(pid, req);
    }

    printFinal();

    return 0;
}