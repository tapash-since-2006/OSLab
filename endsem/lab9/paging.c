#include <stdio.h>

int main() {
    int pageSize, numPages;
    int logicalAddr;

    printf("Enter page size (in bytes): ");
    scanf("%d", &pageSize);

    printf("Enter number of pages: ");
    scanf("%d", &numPages);

    // Page table: maps page number → frame number
    // For simulation, we assign frames 0,1,2,... to pages 0,1,2,...
    int pageTable[numPages];
    printf("\nEnter frame number for each page:\n");
    for (int i = 0; i < numPages; i++) {
        printf("Page %d -> Frame: ", i);
        scanf("%d", &pageTable[i]);
    }

    printf("\nEnter logical address (-1 to stop): ");
scanf("%d", &logicalAddr);

while (logicalAddr != -1) {
    if (logicalAddr < 0 || logicalAddr >= pageSize * numPages) {
        printf("Invalid address (out of range)\n");
    } else {
        int pageNo   = logicalAddr / pageSize;
        int offset   = logicalAddr % pageSize;
        int frameNo  = pageTable[pageNo];
        int physAddr = (frameNo * pageSize) + offset;

        printf("Logical Address : %d\n", logicalAddr);
        printf("Page Number     : %d\n", pageNo);
        printf("Page Offset     : %d\n", offset);
        printf("Frame Number    : %d\n", frameNo);
        printf("Physical Address: %d\n\n", physAddr);
    }

    printf("Enter logical address (-1 to stop): ");
    scanf("%d", &logicalAddr);   // read next value at the bottom of the loop
}
    return 0;
}