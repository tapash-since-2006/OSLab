#include <stdio.h>
#include <stdbool.h>

#define MAX_P 10

int n;
int avail;
int maxDemand[MAX_P];
int alloc[MAX_P];
int need[MAX_P];

void calculateNeed() {
    for (int i = 0; i < n; i++)
        need[i] = maxDemand[i] - alloc[i];
}

void printState() {
    printf("\n%-5s %-10s %-10s %-10s\n", "Proc", "Alloc", "Max", "Need");
    for (int i = 0; i < n; i++) {
        printf("P%-4d %-10d %-10d %-10d\n", i, alloc[i], maxDemand[i], need[i]);
    }
    printf("Available: %d\n", avail);
}

bool isSafe(int safeSeq[]) {
    int work = avail;
    bool finish[MAX_P] = {false};

    int count = 0;

    while (count < n) {
        bool found = false;

        for (int i = 0; i < n; i++) {
            if (!finish[i] && need[i] <= work) {
                printf("P%d can execute. Running...\n", i);

                work += alloc[i];
                finish[i] = true;
                safeSeq[count++] = i;
                found = true;
            }
        }

        if (!found) {
            printf("System is UNSAFE\n");
            return false;
        }
    }

    return true;
}

bool requestResource(int pid, int req) {
    printf("\n>>> P%d requesting: %d\n", pid, req);

    if (req > need[pid]) {
        printf("DENIED: Exceeds max need\n");
        return false;
    }

    if (req > avail) {
        printf("WAIT: Not enough available\n");
        return false;
    }

    // allocate temporarily
    avail -= req;
    alloc[pid] += req;
    need[pid] -= req;

    int safeSeq[MAX_P];

    if (isSafe(safeSeq)) {
        printf("GRANTED. Safe sequence: ");
        for (int i = 0; i < n; i++)
            printf("P%d ", safeSeq[i]);
        printf("\n");

        printState();
        return true;
    } else {
        // rollback
        avail += req;
        alloc[pid] -= req;
        need[pid] += req;

        printf("DENIED: Unsafe state\n");
        return false;
    }
}

int main() {
    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter Allocation:\n");
    for (int i = 0; i < n; i++) {
        printf("P%d: ", i);
        scanf("%d", &alloc[i]);
    }

    printf("Enter Max:\n");
    for (int i = 0; i < n; i++) {
        printf("P%d: ", i);
        scanf("%d", &maxDemand[i]);
    }

    printf("Enter Available: ");
    scanf("%d", &avail);

    calculateNeed();

    printf("\n(a) Need:\n");
    for (int i = 0; i < n; i++)
        printf("P%d: %d\n", i, need[i]);

    printf("\n(b) Safety Check\n");
    int safeSeq[MAX_P];
    if (isSafe(safeSeq)) {
        printf("SAFE. Sequence: ");
        for (int i = 0; i < n; i++)
            printf("P%d ", safeSeq[i]);
        printf("\n");
    }

    int choice;
    do {
        int pid, req;

        printf("\nEnter process: ");
        scanf("%d", &pid);

        printf("Enter request: ");
        scanf("%d", &req);

        requestResource(pid, req);

        printf("Another? (1/0): ");
        scanf("%d", &choice);

    } while (choice == 1);

    return 0;
}