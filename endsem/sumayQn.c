#include <stdio.h>

#define MAX 50
#define PAGE_SIZE 256

// ----------------- Utility Functions -----------------

int find_victim(int frames[], int nf, int pages[], int n, int cur) {
    int victim = 0, farthest = -1;

    for (int i = 0; i < nf; i++) {
        int j;
        for (j = cur + 1; j < n; j++) {
            if (frames[i] == pages[j]) {
                if (j > farthest) {
                    farthest = j;
                    victim = i;
                }
                break;
            }
        }
        if (j == n) return i;
    }
    return victim;
}

void print_frames(int frames[], int nf) {
    printf("[ ");
    for (int i = 0; i < nf; i++) {
        if (frames[i] == -1) printf("- ");
        else printf("%d ", frames[i]);
    }
    printf("]");
}

// ----------------- Part (a) -----------------

void partA(int logical[], int pages[], int n) {
    printf("\nAddress\tPage No\tOffset\n");

    for (int i = 0; i < n; i++) {
        pages[i] = logical[i] / PAGE_SIZE;
        int offset = logical[i] % PAGE_SIZE;
        printf("%d\t%d\t%d\n", logical[i], pages[i], offset);
    }
}

// ----------------- Part (b) -----------------

void partB(int pages[], int n, float *fault_rate, float *hit_ratio) {
    int nf;

    printf("\nEnter number of frames: ");
    scanf("%d", &nf);

    int frames[nf];
    for (int i = 0; i < nf; i++) frames[i] = -1;

    int faults = 0, hits = 0, filled = 0;

    printf("\nStep\tPage\tFrames\t\tResult\n");

    for (int i = 0; i < n; i++) {
        int pg = pages[i];
        int found = 0;

        for (int j = 0; j < nf; j++) {
            if (frames[j] == pg) {
                found = 1;
                break;
            }
        }

        if (found) {
            hits++;
        } else {
            faults++;

            if (filled < nf) {
                frames[filled++] = pg;
            } else {
                int v = find_victim(frames, nf, pages, n, i);
                frames[v] = pg;
            }
        }

        printf("%d\t%d\t", i + 1, pg);
        print_frames(frames, nf);

        if (found) printf("\tHIT\n");
        else printf("\tFAULT\n");
    }

    *hit_ratio = (float)hits / n;
    *fault_rate = (float)faults / n;

    printf("\nTotal Hits   = %d", hits);
    printf("\nTotal Faults = %d", faults);
    printf("\nHit Ratio    = %.2f\n", *hit_ratio);
}

// ----------------- Part (c) -----------------

void partC(float fault_rate) {
    float m, p, EAT;

    printf("\nEnter Memory Access Time (m in ns): ");
    scanf("%f", &m);

    printf("Enter Page Fault Service Time (p in ns): ");
    scanf("%f", &p);

    EAT = (1 - fault_rate) * m + fault_rate * p;

    printf("\nEAT = %.2f ns\n", EAT);
}

// ----------------- Question 2 -----------------

void question2() {
    int addr;

    printf("\nEnter logical address (0 - 65535): ");
    scanf("%d", &addr);

    if (addr < 0 || addr > 65535) {
        printf("Invalid address\n");
        return;
    }

    int page = addr / PAGE_SIZE;
    int offset = addr % PAGE_SIZE;

    printf("\nPage Number = %d\n", page);
    printf("Offset      = %d\n", offset);

    printf("\nBits for Page Number = 8\n");
    printf("Bits for Offset      = 8\n");
}

// ----------------- MAIN -----------------

int main() {
    int n, logical[MAX], pages[MAX];

    printf("Enter number of logical addresses: ");
    scanf("%d", &n);

    printf("Enter logical addresses:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &logical[i]);
    }

    // Part (a)
    partA(logical, pages, n);

    // Part (b)
    float fault_rate, hit_ratio;
    partB(pages, n, &fault_rate, &hit_ratio);

    // Part (c)
    partC(fault_rate);

    // Question 2
    question2();

    return 0;
}