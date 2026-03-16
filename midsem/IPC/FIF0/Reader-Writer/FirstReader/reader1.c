#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd;
    int value;
    char *fifo = "rw_fifo";

    fd = open(fifo, O_RDONLY);
    read(fd, &value, sizeof(value));
    printf("Reader 1 read: %d\n", value);

    close(fd);
    return 0;
}