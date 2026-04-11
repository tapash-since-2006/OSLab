#include <stdio.h>
#include <string.h>

#define MAX 25

// Structure to hold hole info
struct hole {
    int hno;          // hole number
    int hsize;        // original size
    int hrem;         // remaining size after allocation
};

// Structure to hold process info
struct process {
    int pno;          // process number
    int psize;        // memory required
    int alloc;        // hole number it was allocated to (-1 if not)
};


struct hole holes[MAX];
struct process procs[MAX];
int nh, np;

void readInput() {
    printf("Enter number of holes: ");
    scanf("%d", &nh);
    for (int i = 0; i < nh; i++) {
        holes[i].hno = i + 1;
        printf("Size of hole %d: ", i + 1);
        scanf("%d", &holes[i].hsize);
        holes[i].hrem = holes[i].hsize;
    }
    printf("Enter number of processes: ");
    scanf("%d", &np);
    for (int i = 0; i < np; i++) {
        procs[i].pno = i + 1;
        printf("Memory required for process %d: ", i + 1);
        scanf("%d", &procs[i].psize);
        procs[i].alloc = -1;  // -1 means not allocated yet
    }
}

// =================== FIRST FIT ===================
void firstFit() {
    // Reset remaining sizes
    for (int i = 0; i < nh; i++) holes[i].hrem = holes[i].hsize;
    for (int i = 0; i < np; i++) procs[i].alloc = -1;

    for (int j = 0; j < np; j++) {           // for each process
        for (int i = 0; i < nh; i++) {        // scan holes from start
            if (holes[i].hrem >= procs[j].psize) {  // first fit found
                procs[j].alloc = holes[i].hno;
                holes[i].hrem -= procs[j].psize;
                break;  // IMPORTANT: stop after first fit
            }
        }
    }
}

// =================== BEST FIT ===================
void bestFit() {
    for (int i = 0; i < nh; i++) holes[i].hrem = holes[i].hsize;
    for (int i = 0; i < np; i++) procs[i].alloc = -1;

    for (int j = 0; j < np; j++) {           // for each process
        int bestIdx = -1;
        int bestSize = 999999;               // track smallest fitting hole
        for (int i = 0; i < nh; i++) {       // scan ALL holes
            if (holes[i].hrem >= procs[j].psize) {
                if (holes[i].hrem < bestSize) {  // smaller than current best?
                    bestSize = holes[i].hrem;
                    bestIdx = i;
                }
            }
        }
        if (bestIdx != -1) {
            procs[j].alloc = holes[bestIdx].hno;
            holes[bestIdx].hrem -= procs[j].psize;
        }
        // NO break here — must scan all holes to find the smallest fit
    }
}

// =================== WORST FIT ===================
void worstFit() {
    for (int i = 0; i < nh; i++) holes[i].hrem = holes[i].hsize;
    for (int i = 0; i < np; i++) procs[i].alloc = -1;

    for (int j = 0; j < np; j++) {
        int worstIdx = -1;
        int worstSize = -1;                  // track LARGEST hole
        for (int i = 0; i < nh; i++) {
            if (holes[i].hrem >= procs[j].psize) {
                if (holes[i].hrem > worstSize) {  // larger than current worst?
                    worstSize = holes[i].hrem;
                    worstIdx = i;
                }
            }
        }
        if (worstIdx != -1) {
            procs[j].alloc = holes[worstIdx].hno;
            holes[worstIdx].hrem -= procs[j].psize;
        }
    }
}

void printResult() {
    printf("\n--- Process Allocation ---\n");
    printf("%-10s %-10s %-15s\n", "Process", "Size", "Allocated Hole");
    for (int i = 0; i < np; i++) {
        if (procs[i].alloc == -1)
            printf("%-10d %-10d %-15s\n", procs[i].pno, procs[i].psize, "Not Allocated");
        else
            printf("%-10d %-10d %-15d\n", procs[i].pno, procs[i].psize, procs[i].alloc);
    }
    printf("\n--- Hole Status ---\n");
    printf("%-5s %-10s %-10s\n", "Hole", "Original", "Remaining");
    for (int i = 0; i < nh; i++)
        printf("%-5d %-10d %-10d\n", holes[i].hno, holes[i].hsize, holes[i].hrem);
}

int main() {
    readInput();

    printf("\n===== FIRST FIT =====");
    firstFit();
    printResult();

    printf("\n===== BEST FIT =====");
    bestFit();
    printResult();

    printf("\n===== WORST FIT =====");
    worstFit();
    printResult();

    return 0;
}