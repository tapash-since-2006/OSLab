#include <stdio.h>

#define MAX_SEG 10

struct segment {
    int base;   
    int limit;  
};

struct segment segTable[MAX_SEG];

int numSeg;

void buildSegmentTable() {
    printf("Enter number of segments: ");
    scanf("%d", &numSeg);
    for (int i = 0; i < numSeg; i++) {
        printf("Segment %d - Base address: ", i);
        scanf("%d", &segTable[i].base);
        printf("Segment %d - Limit (size): ", i);
        scanf("%d", &segTable[i].limit);
    }
}

void translateAddress(int segNo, int offset) {
    if (segNo < 0 || segNo >= numSeg) {
        printf("Error: Invalid segment number %d\n", segNo);
        return;
    }
    if (offset >= segTable[segNo].limit) {
        // offset exceeds segment size → protection violation
        printf("Segmentation Fault! Offset %d >= Limit %d for segment %d\n",
               offset, segTable[segNo].limit, segNo);
        return;
    }
    int physAddr = segTable[segNo].base + offset;
    printf("Logical  : <seg=%d, offset=%d>\n", segNo, offset);
    printf("Physical : %d (base=%d + offset=%d)\n\n",
           physAddr, segTable[segNo].base, offset);
}

int main() {
    buildSegmentTable();

    int s, d;
    printf("\nEnter logical address as <segment offset> (-1 -1 to quit):\n");
    scanf("%d %d", &s, &d);

    while (s != -1 && d != -1) {
        translateAddress(s, d);
        printf("Enter logical address: ");
        scanf("%d %d", &s, &d);
    }

    return 0;
}