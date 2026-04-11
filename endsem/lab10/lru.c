#include <stdio.h>

void lru(int pages[], int n, int nf) {
    int frames[nf], lastUsed[nf];
    int pageFaults = 0, filled = 0;

    for (int i = 0; i < nf; i++) { frames[i] = -1; lastUsed[i] = -1; }

    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int hit = 0, hitSlot = -1;

        for (int f = 0; f < nf; f++) {
            if (frames[f] == page) { hit = 1; hitSlot = f; break; }
        }

        if (hit) {
            lastUsed[hitSlot] = i;      // update its last-used time
            printf("Page %d -> HIT    frames: ", page);
        } else {
            pageFaults++;
            if (filled < nf) {
                // frames not full — fill next empty slot
                frames[filled] = page;
                lastUsed[filled] = i;
                filled++;
            } else {
                // find LRU victim: frame with smallest lastUsed time
                int victimSlot = 0;
                for (int f = 1; f < nf; f++) {
                    if (lastUsed[f] < lastUsed[victimSlot]) victimSlot = f;
                }
                printf("  (evicting page %d) ", frames[victimSlot]);
                frames[victimSlot] = page;
                lastUsed[victimSlot] = i;
            }
            printf("Page %d -> FAULT  frames: ", page);
        }

        for (int f = 0; f < nf; f++)
            printf("%2d ", frames[f] == -1 ? -1 : frames[f]);
        printf("\n");
    }

    int hits = n - pageFaults;
    printf("Total page faults (LRU): %d\n", pageFaults);
    printf("Total hits: %d\n", hits);
    printf("Hit ratio: %.2f%%\n", (float)hits / n * 100);
}

int main() {
    int n, nf;
    printf("Number of pages: "); scanf("%d", &n);
    int pages[n];
    for (int i = 0; i < n; i++) scanf("%d", &pages[i]);
    printf("Number of frames: "); scanf("%d", &nf);
    lru(pages, n, nf);
    return 0;
}