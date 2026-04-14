#include <stdio.h>

void lifo(int pages[], int n, int nf) {
    int frames[nf];
    int top = -1;  // points to last inserted
    int pageFaults = 0;

    for (int i = 0; i < nf; i++) frames[i] = -1;

    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int hit = 0;

        // Check hit
        for (int f = 0; f < nf; f++) {
            if (frames[f] == page) {
                hit = 1;
                break;
            }
        }

        if (!hit) {
            pageFaults++;

            if (top < nf - 1) {
                // still space available
                top++;
                frames[top] = page;
            } else {
                // replace last inserted (LIFO)
                frames[top] = page;
            }

            printf("Page %d -> FAULT  frames: ", page);
        } else {
            printf("Page %d -> HIT    frames: ", page);
        }

        for (int f = 0; f < nf; f++)
            printf("%2d ", frames[f] == -1 ? -1 : frames[f]);

        printf("\n");
    }

    printf("Total page faults (LIFO): %d\n", pageFaults);
}

int main() {
    int n, nf;
    printf("Number of pages: ");
    scanf("%d", &n);

    int pages[n];
    for (int i = 0; i < n; i++)
        scanf("%d", &pages[i]);

    printf("Number of frames: ");
    scanf("%d", &nf);

    lifo(pages, n, nf);

    return 0;
}