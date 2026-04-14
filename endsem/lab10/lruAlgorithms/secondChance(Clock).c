#include <stdio.h>

void secondChance(int pages[], int n, int nf) {
    int frames[nf], refBit[nf];
    int filled = 0, pageFaults = 0;
    int hand = 0;  // clock hand — points to next eviction candidate

    for (int i = 0; i < nf; i++) { frames[i] = -1; refBit[i] = 0; }

    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int hit = 0;

        for (int f = 0; f < nf; f++) {
            if (frames[f] == page) {
                hit = 1;
                refBit[f] = 1;  // page was used — set reference bit
                break;
            }
        }

        if (!hit) {
            pageFaults++;
            if (filled < nf) {
                // frames not full yet
                frames[filled] = page;
                refBit[filled] = 1;
                filled++;
            } else {
                // clock algorithm: spin until we find refBit = 0
                while (refBit[hand] == 1) {
                    refBit[hand] = 0;           // give second chance — clear bit
                    hand = (hand + 1) % nf;     // move clock hand forward
                }
                // hand now points to a page with refBit = 0 — evict it
                printf("  (evicting page %d) ", frames[hand]);
                frames[hand] = page;
                refBit[hand] = 1;               // newly loaded page gets refBit = 1
                hand = (hand + 1) % nf;         // advance hand
            }
            printf("Page %d -> FAULT  frames: ", page);
        } else {
            printf("Page %d -> HIT    frames: ", page);
        }

        for (int f = 0; f < nf; f++) printf("%2d ", frames[f] == -1 ? -1 : frames[f]);
        printf("\n");
    }
    printf("Total page faults: %d\n", pageFaults);
}

int main() {
    int n, nf;
    printf("Number of pages: "); scanf("%d", &n);
    int pages[n];
    for (int i = 0; i < n; i++) scanf("%d", &pages[i]);
    printf("Number of frames: "); scanf("%d", &nf);
    secondChance(pages, n, nf);
    return 0;
}