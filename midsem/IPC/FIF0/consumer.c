#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fd;
    int nums[4];
    char *fifo = "myfifo";

    fd = open(fifo, O_RDONLY);

    read(fd, nums, sizeof(nums));

    printf("Consumer received numbers:\n");
    for(int i=0;i<4;i++)
        printf("%d ", nums[i]);

    printf("\n");

    close(fd);

    return 0;
}