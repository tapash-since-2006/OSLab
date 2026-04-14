// #include <stdio.h>

// void enhancedSecondChance(int pages[], int n, int nf, int modified[]) {
//     int frames[nf], R[nf], M[nf];
//     int filled = 0, pageFaults = 0;
//     int hand = 0;

//     // initialize
//     for (int i = 0; i < nf; i++) {
//         frames[i] = -1;
//         R[i] = 0;
//         M[i] = 0;
//     }

//     for (int i = 0; i < n; i++) {
//         int page = pages[i];
//         int hit = 0;

//         // 🔹 Reset R bits periodically (simulate "last interval")
//         if (i % 3 == 0) {   // you can change interval (3 is fine for lab)
//             for (int f = 0; f < nf; f++)
//                 R[f] = 0;
//         }

//         // 🔹 Check HIT
//         for (int f = 0; f < nf; f++) {
//             if (frames[f] == page) {
//                 hit = 1;
//                 R[f] = 1;                     // recently used
//                 if (modified[i]) M[f] = 1;    // mark dirty if write
//                 break;
//             }
//         }

//         // 🔹 Page Fault
//         if (!hit) {
//             pageFaults++;

//             // Case 1: Empty frame
//             if (filled < nf) {
//                 frames[filled] = page;
//                 R[filled] = 1;
//                 M[filled] = modified[i] ? 1 : 0;
//                 filled++;
//             } 
//             else {
//                 int victim = -1;

//                 // 🔁 4-pass search
//                 for (int pass = 0; pass < 4 && victim == -1; pass++) {
//                     for (int f = 0; f < nf; f++) {
//                         int idx = (hand + f) % nf;

//                         // Pass 1: (0,0)
//                         if (pass == 0 && R[idx] == 0 && M[idx] == 0) {
//                             victim = idx;
//                             break;
//                         }

//                         // Pass 2: (0,1)
//                         if (pass == 1 && R[idx] == 0 && M[idx] == 1) {
//                             victim = idx;
//                             break;
//                         }

//                         // Pass 3: clear R bits
//                         if (pass == 2) {
//                             R[idx] = 0;
//                         }

//                         // Pass 4: (0,0) again
//                         if (pass == 3 && R[idx] == 0 && M[idx] == 0) {
//                             victim = idx;
//                             break;
//                         }
//                     }
//                 }

//                 if (victim == -1)
//                     victim = hand;

//                 printf("  (evicting page %d R=%d M=%d) ",
//                        frames[victim], R[victim], M[victim]);

//                 frames[victim] = page;
//                 R[victim] = 1;
//                 M[victim] = modified[i] ? 1 : 0;

//                 hand = (victim + 1) % nf;
//             }

//             printf("Page %d -> FAULT  frames: ", page);
//         } 
//         else {
//             printf("Page %d -> HIT    frames: ", page);
//         }

//         // 🔹 Display frames
//         for (int f = 0; f < nf; f++) {
//             if (frames[f] == -1)
//                 printf(" - ");
//             else
//                 printf("%d(R=%d,M=%d) ", frames[f], R[f], M[f]);
//         }
//         printf("\n");
//     }

//     printf("\nTotal page faults: %d\n", pageFaults);
// }

// int main() {
//     int n, nf;

//     printf("Number of pages: ");
//     scanf("%d", &n);

//     int pages[n], modified[n];

//     printf("Enter page references: ");
//     for (int i = 0; i < n; i++)
//         scanf("%d", &pages[i]);

//     printf("Enter modified bits (1=write, 0=read): ");
//     for (int i = 0; i < n; i++)
//         scanf("%d", &modified[i]);

//     printf("Number of frames: ");
//     scanf("%d", &nf);

//     enhancedSecondChance(pages, n, nf, modified);

//     return 0;
// }


#include <stdio.h>

void enhancedSecondChance(int pages[], int n, int nf, int modified[]) {
    int frames[nf], R[nf], M[nf];
    int filled = 0, pageFaults = 0;
    int hand = 0;

    // initialize
    for (int i = 0; i < nf; i++) {
        frames[i] = -1;
        R[i] = 0;
        M[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int hit = 0;

        // check HIT
        for (int f = 0; f < nf; f++) {
            if (frames[f] == page) {
                hit = 1;
                R[f] = 1;
                if (modified[i]) M[f] = 1;
                break;
            }
        }

        // PAGE FAULT
        if (!hit) {
            pageFaults++;

            // empty slot
            if (filled < nf) {
                frames[filled] = page;
                R[filled] = 1;
                M[filled] = modified[i];
                filled++;
            } 
            else {
                int victim = -1;

                // CLOCK SEARCH (simple 2-bit priority)
                for (int count = 0; count < nf; count++) {
                    int idx = (hand + count) % nf;

                    // (0,0) best
                    if (R[idx] == 0 && M[idx] == 0) {
                        victim = idx;
                        break;
                    }
                }

                // if not found (0,0), check (0,1)
                if (victim == -1) {
                    for (int count = 0; count < nf; count++) {
                        int idx = (hand + count) % nf;

                        if (R[idx] == 0 && M[idx] == 1) {
                            victim = idx;
                            break;
                        }
                    }
                }

                // if still not found, take first available
                if (victim == -1) {
                    victim = hand;
                }

                printf("Evicting page %d (R=%d M=%d)\n",
                       frames[victim], R[victim], M[victim]);

                frames[victim] = page;
                R[victim] = 1;
                M[victim] = modified[i];

                hand = (victim + 1) % nf;
            }

            printf("Page %d -> FAULT | Frames: ", page);
        } 
        else {
            printf("Page %d -> HIT   | Frames: ", page);
        }

        // display
        for (int f = 0; f < nf; f++) {
            if (frames[f] == -1)
                printf(" - ");
            else
                printf("%d ", frames[f]);
        }
        printf("\n");
    }

    printf("\nTotal Page Faults: %d\n", pageFaults);
}

int main() {
    int n, nf;

    printf("Number of pages: ");
    scanf("%d", &n);

    int pages[n], modified[n];

    printf("Enter page references: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &pages[i]);

    printf("Enter modified bits (0/1): ");
    for (int i = 0; i < n; i++)
        scanf("%d", &modified[i]);

    printf("Number of frames: ");
    scanf("%d", &nf);

    enhancedSecondChance(pages, n, nf, modified);

    return 0;
}