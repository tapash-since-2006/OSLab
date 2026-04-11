#include <stdio.h>

// returns the index of the frame to evict
int findOptVictim(int frames[], int nf, int pages[], int curr, int n) {
    int farthest = -1, victimSlot = 0;

    for (int f = 0; f < nf; f++) {
        int nextUse = -1;
        // search ahead in the reference string for frames[f]
        for (int j = curr + 1; j < n; j++) {
            if (pages[j] == frames[f]) { nextUse = j; break; }
        }
        // if this page never appears again, it's the best to evict
        if (nextUse == -1) return f;
        // otherwise track which is used furthest in the future
        if (nextUse > farthest) { farthest = nextUse; victimSlot = f; }
    }
    return victimSlot;
}

void optimal(int pages[], int n, int nf) {
    int frames[nf];
    int pageFaults = 0, filled = 0;

    for (int i = 0; i < nf; i++) frames[i] = -1;

    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int hit = 0;

        for (int f = 0; f < nf; f++) {
            if (frames[f] == page) { hit = 1; break; }
        }

        if (!hit) {
            pageFaults++;
            if (filled < nf) {
                // frames not full yet — just fill next slot
                frames[filled++] = page;
            } else {
                // all full — find optimal victim
                int victim = findOptVictim(frames, nf, pages, i, n);
                printf("  (evicting page %d) ", frames[victim]);
                frames[victim] = page;
            }
            printf("Page %d -> FAULT  frames: ", page);
        } else {
            printf("Page %d -> HIT    frames: ", page);
        }

        for (int f = 0; f < nf; f++)
            printf("%2d ", frames[f] == -1 ? -1 : frames[f]);
        printf("\n");
    }
    printf("Total page faults (Optimal): %d\n", pageFaults);
}


int main() {
    int n, nf;
    printf("Number of pages: "); scanf("%d", &n);
    int pages[n];
    for (int i = 0; i < n; i++) scanf("%d", &pages[i]);
    printf("Number of frames: "); scanf("%d", &nf);
    optimal(pages, n, nf);
    return 0;
}