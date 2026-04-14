#include <stdio.h>

#define BITS 8

// function to print 8-bit binary
void printBinary(int num) {
    for (int i = BITS - 1; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
    }
}

void mruReferenceRegister(int pages[], int n, int nf) {
    int frames[nf], refByte[nf];
    int filled = 0, pageFaults = 0, hits = 0;

    // initialize
    for (int i = 0; i < nf; i++) {
        frames[i] = -1;
        refByte[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int hit = 0;

        // shift all reference registers
        for (int f = 0; f < nf; f++) {
            refByte[f] >>= 1;
        }

        // check if page is already present
        for (int f = 0; f < nf; f++) {
            if (frames[f] == page) {
                hit = 1;
                refByte[f] |= (1 << (BITS - 1)); // set MSB
                break;
            }
        }

        if (hit) {
            hits++;
        } else {
            pageFaults++;

            if (filled < nf) {
                frames[filled] = page;
                refByte[filled] = (1 << (BITS - 1));
                filled++;
            } else {
                // MRU: find page with MAX refByte
                int victim = 0;
                for (int f = 1; f < nf; f++) {
                    if (refByte[f] > refByte[victim]) {
                        victim = f;
                    }
                }

                frames[victim] = page;
                refByte[victim] = (1 << (BITS - 1));
            }
        }

        // OUTPUT (as per PDF)
        printf("\nAfter page %d:\n", page);
        for (int f = 0; f < nf; f++) {
            if (frames[f] == -1) {
                printf("- : ");
            } else {
                printf("%d : ", frames[f]);
            }
            printBinary(refByte[f]);
            printf("\n");
        }

        if (hit)
            printf("Hit\n");
        else
            printf("Fault\n");
    }

    printf("\nTotal Hits = %d\n", hits);
    printf("Total Faults = %d\n", pageFaults);
}

int main() {
    int n, nf;

    printf("Number of pages: ");
    scanf("%d", &n);

    int pages[n];
    printf("Enter pages: ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &pages[i]);
    }

    printf("Number of frames: ");
    scanf("%d", &nf);

    mruReferenceRegister(pages, n, nf);

    return 0;
}