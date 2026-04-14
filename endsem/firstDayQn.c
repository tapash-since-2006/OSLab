#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#define max 1000

int nf;
int head;
int page_number[max];
int cyl_pos[max];
int req[max];

int abs_val(int x){
    if(x<0) return -x;
    return x;
}

void LSTF(int n){
    int pos = head;
    int thm = 0;
    int count = 0;
    bool visited[max] = {false};

    printf("\n--- Phase 1: LSTF Disk Scheduling ---\n");
    printf("Head = %d\n\n", head);

    // Print initial distances
    printf("Distances: ");
    for(int i = 0; i < n; i++)
        printf("|%d-%d|=%d  ", cyl_pos[i], head, abs_val(pos - cyl_pos[i]));
    printf("\n\n");

    printf("Service order (largest seek first):\n");

    while(count < n){
        int largest = -1;
        int idx = -1;

        for(int i = 0; i < n; i++){
            if(!visited[i] && (abs_val(pos - cyl_pos[i]) > largest)){
                largest = abs_val(pos - cyl_pos[i]);
                idx = i;
            }
        }

        thm += largest;
        // Print each step
        printf("%d. Cyl %d (dist %d) -> Page %d   head=%d\n",
               count + 1, cyl_pos[idx], largest, page_number[idx], cyl_pos[idx]);

        pos = cyl_pos[idx];
        visited[idx] = 1;
        req[count] = page_number[idx];
        count++;
    }

    printf("\nTotal Seek Distance: %d\n", thm);

    // Print LSTF order summary
    printf("\nLSTF Page Order: ");
    for(int i = 0; i < n; i++)
        printf("%d ", req[i]);
    printf("\n");
}

void mfu(int n){
    int frames[nf];
    int freq[nf];
    int timestamp[nf];
    int pageFault = 0;
    int filled = 0;
    int timer = 0;

    for(int i = 0; i < nf; i++){
        frames[i] = -1;
        freq[i] = 0;
        timestamp[i] = 0;
    }

    printf("\n--- Phase 2: MFU Page Replacement ---\n");
    printf("Frames: %d\n\n", nf);

    for(int i = 0; i < n; i++){
        int page = req[i];
        int hit = 0;
        int hitSlot = -1;

        for(int f = 0; f < nf; f++){
            if(frames[f] == page){
                hit = 1;
                hitSlot = f;
                break;
            }
        }

        if(hit){
            freq[hitSlot]++;
            timestamp[hitSlot] = timer++;
            printf("Page %d -> Hit    | Frames: ", page);
        } else {
            pageFault++;

            if(filled < nf){
                frames[filled] = page;
                freq[filled] = 1;
                timestamp[filled] = timer++;
                filled++;
            } else {
                int victimSlot = 0;
                for(int f = 1; f < nf; f++){
                    if(freq[f] > freq[victimSlot]){
                        victimSlot = f;
                    } else if(freq[f] == freq[victimSlot]){
                        if(timestamp[f] < timestamp[victimSlot]){
                            victimSlot = f;
                        }
                    }
                }
                printf("Evicted page %d\n", frames[victimSlot]);
                frames[victimSlot] = page;
                freq[victimSlot] = 1;
                timestamp[victimSlot] = timer++;
            }
            printf("Page %d -> Fault  | Frames: ", page);
        }

        // Print frame state
        for(int f = 0; f < nf; f++){
            if(frames[f] == -1) printf("[ ] ");
            else printf("[%d] ", frames[f]);
        }

        // Print freq state
        printf(" freq: {");
        int first = 1;
        for(int f = 0; f < nf; f++){
            if(frames[f] != -1){
                if(!first) printf(", ");
                printf("%d:%d", frames[f], freq[f]);
                first = 0;
            }
        }
        printf("}\n");
    }

    printf("\nTotal Page Faults: %d\n", pageFault);
}

int main(){
    int choice = 1;
    int n = 0;

    printf("Enter the number of frames: ");
    scanf("%d", &nf);

    printf("Enter the initial head position: ");
    scanf("%d", &head);

    printf("Enter -1 to exit and 1 to continue: ");
    scanf("%d", &choice);

    while(choice != -1){
        printf("Enter page_number and cylinder_pos: ");
        scanf("%d %d", &page_number[n], &cyl_pos[n]);
        n++;

        printf("Enter -1 to exit and 1 to continue: ");
        scanf("%d", &choice);
    }

    LSTF(n);
    mfu(n);

    return 0;
}