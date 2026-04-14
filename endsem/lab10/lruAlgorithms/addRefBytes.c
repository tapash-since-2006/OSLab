#include <stdio.h>

#define BITS 8

void additionalReferenceByte(int pages[], int n, int nf) {
    int frames[nf], refByte[nf], timeStamp[nf];
    int filled = 0, pageFaults = 0;
    int clock = 0;

    for (int i = 0; i < nf; i++) {
        frames[i] = -1;
        refByte[i] = 0;
        timeStamp[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int hit = 0;

        // shift reference bytes right
        for (int f = 0; f < nf; f++) {
            refByte[f] >>= 1;
        }

        // check hit
        for (int f = 0; f < nf; f++) {
            if (frames[f] == page) {
                hit = 1;
                refByte[f] |= (1<<(BITS-1));   // set MSB
                break;
            }
        }

        if (!hit) {
            pageFaults++;

            if (filled < nf) {
                frames[filled] = page;
                refByte[filled] = (1<<(BITS-1));
                timeStamp[filled] = clock++;
                filled++;
            } 
            else {
                // find victim (min refByte + FIFO tie-break)
                int victim = 0;

                for (int f = 1; f < nf; f++) {
                    if (refByte[f] < refByte[victim]) {
                        victim = f;
                    } 
                    else if (refByte[f] == refByte[victim]) {
                        // FIFO tie-break
                        if (timeStamp[f] < timeStamp[victim]) {
                            victim = f;
                        }
                    }
                }

                printf("  (evicting page %d) ", frames[victim]);

                frames[victim] = page;
                refByte[victim] =(1<<(BITS-1));
                timeStamp[victim] = clock++;
            }

            printf("Page %d -> FAULT  frames: ", page);
        } 
        else {
            printf("Page %d -> HIT    frames: ", page);
        }

        for (int f = 0; f < nf; f++) {
            printf("%2d ", frames[f]);
        }
        printf("\n");
    }

    printf("Total page faults: %d\n", pageFaults);
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

    additionalReferenceByte(pages, n, nf);

    return 0;
}