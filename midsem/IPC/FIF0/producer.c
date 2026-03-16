#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fd;
    int nums[4];
    char *fifo = "myfifo";

    mkfifo(fifo, 0666);

    printf("Enter 4 integers:\n");
    for(int i=0;i<4;i++)
        scanf("%d",&nums[i]);

    fd = open(fifo, O_WRONLY);

    write(fd, nums, sizeof(nums));

    close(fd);

    return 0;
}