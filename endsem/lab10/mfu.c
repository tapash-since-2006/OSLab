#include <stdio.h>

void mfu(int pages[], int n, int nf) {
    int frames[nf], freq[nf], time[nf];
    int pageFaults = 0, filled = 0;
    int timer = 0;

    for (int i = 0; i < nf; i++) {
        frames[i] = -1;
        freq[i] = 0;
        time[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int hit = 0, hitSlot = -1;

        // Check HIT
        for (int f = 0; f < nf; f++) {
            if (frames[f] == page) {
                hit = 1;
                hitSlot = f;
                break;
            }
        }

        if (hit) {
            freq[hitSlot]++;        // increase frequency
            time[hitSlot] = timer++; // update recency
            printf("Page %d -> HIT    frames: ", page);
        } else {
            pageFaults++;

            if (filled < nf) {
                // Empty frame available
                frames[filled] = page;
                freq[filled] = 1;
                time[filled] = timer++;
                filled++;
            } else {
                // Find MFU victim (max frequency)
                int victim = 0;

                for (int f = 1; f < nf; f++) {
                    if (freq[f] > freq[victim]) {
                        victim = f;
                    }
                    else if (freq[f] == freq[victim]) {
                        // Tie → use LRU (older time)
                        if (time[f] < time[victim])
                            victim = f;
                    }
                }

                printf("(evicting page %d) ", frames[victim]);

                frames[victim] = page;
                freq[victim] = 1;
                time[victim] = timer++;
            }

            printf("Page %d -> FAULT  frames: ", page);
        }

        // Print frames
        for (int f = 0; f < nf; f++)
            printf("%2d ", frames[f] == -1 ? -1 : frames[f]);
        printf("\n");
    }

    int hits = n - pageFaults;
    printf("\nTotal page faults (MFU): %d\n", pageFaults);
    printf("Total hits: %d\n", hits);
    printf("Hit ratio: %.2f%%\n", (float)hits / n * 100);
}

int main() {
    int n, nf;

    printf("Number of pages: ");
    scanf("%d", &n);

    int pages[n];
    printf("Enter page reference string:\n");
    for (int i = 0; i < n; i++)
        scanf("%d", &pages[i]);

    printf("Number of frames: ");
    scanf("%d", &nf);

    mfu(pages, n, nf);

    return 0;
}