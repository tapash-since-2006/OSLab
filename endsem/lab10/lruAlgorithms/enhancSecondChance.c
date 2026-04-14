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

    // Initialize
    for (int i = 0; i < nf; i++) {
        frames[i] = -1;
        R[i] = 0;
        M[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int hit = 0;

        // Check HIT
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

            // Empty slot available
            if (filled < nf) {
                frames[filled] = page;
                R[filled] = 1;
                M[filled] = modified[i];
                filled++;
            } else {
                int victim = -1;

                // Pass 1: (0,0) - not recently used, not modified (best)
                for (int count = 0; count < nf; count++) {
                    int idx = (hand + count) % nf;
                    if (R[idx] == 0 && M[idx] == 0) {
                        victim = idx;
                        break;
                    }
                }

                if (victim == -1) {
                    // Pass 2: (0,1) - not recently used but modified
                    for (int count = 0; count < nf; count++) {
                        int idx = (hand + count) % nf;
                        if (R[idx] == 0 && M[idx] == 1) {
                            victim = idx;
                            break;
                        }
                    }
                }

                if (victim == -1) {
                    // Pass 3: (1,0) - recently used but not modified
                    for (int count = 0; count < nf; count++) {
                        int idx = (hand + count) % nf;
                        if (R[idx] == 1 && M[idx] == 0) {
                            victim = idx;
                            break;
                        }
                    }
                }

                if (victim == -1) {
                    // Pass 4: (1,1) - recently used and modified (worst)
                    for (int count = 0; count < nf; count++) {
                        int idx = (hand + count) % nf;
                        if (R[idx] == 1 && M[idx] == 1) {
                            victim = idx;
                            break;
                        }
                    }
                }

                printf("Evicting page %d (R=%d, M=%d)\n",
                       frames[victim], R[victim], M[victim]);

                frames[victim] = page;
                R[victim] = 1;
                M[victim] = modified[i];

                hand = (victim + 1) % nf;
            }

            printf("Page %d -> FAULT | Frames: ", page);
        } else {
            printf("Page %d -> HIT   | Frames: ", page);
        }

        // Display current frames
        for (int f = 0; f < nf; f++) {
            if (frames[f] == -1)
                printf(" - ");
            else
                printf("%2d ", frames[f]);
        }

        // Display R and M bits
        printf("| R: ");
        for (int f = 0; f < nf; f++) printf("%d ", R[f]);
        printf("| M: ");
        for (int f = 0; f < nf; f++) printf("%d ", M[f]);
        printf("\n");
    }

    printf("\nTotal Page Faults: %d\n", pageFaults);
}

int main() {
    int n, nf;

    printf("Enter number of page references: ");
    scanf("%d", &n);

    int pages[n], modified[n];

    printf("Enter page references: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &pages[i]);

    printf("Enter modified bits (0/1) for each reference: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &modified[i]);

    printf("Enter number of frames: ");
    scanf("%d", &nf);

    printf("\n--- Enhanced Second-Chance Algorithm ---\n\n");

    enhancedSecondChance(pages, n, nf, modified);

    return 0;
}