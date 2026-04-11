#include <stdio.h>

void fifo(int pages[], int n, int nf) {
    int frames[nf];
    int front = 0, rear = 0;
    int pageFaults = 0;

    for (int i = 0; i < nf; i++) frames[i] = -1;

    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int hit = 0;

        for (int f = 0; f < nf; f++) {
            if (frames[f] == page) { hit = 1; break; }
        }

        if (!hit) {
            pageFaults++;
            if (rear - front < nf) {
                frames[rear % nf] = page;
            } else {
                frames[front % nf] = page;
                front++;
            }
            rear++;
            printf("Page %d -> FAULT  frames: ", page);
        } else {
            printf("Page %d -> HIT    frames: ", page);
        }

        for (int f = 0; f < nf; f++)
            printf("%2d ", frames[f] == -1 ? -1 : frames[f]);
        printf("\n");
    }
    printf("Total page faults (FIFO): %d\n", pageFaults);
}


int main() {
    int n, nf;
    printf("Number of pages: "); scanf("%d", &n);
    int pages[n];
    for (int i = 0; i < n; i++) scanf("%d", &pages[i]);
    printf("Number of frames: "); scanf("%d", &nf);
    fifo(pages, n, nf);
    return 0;
}