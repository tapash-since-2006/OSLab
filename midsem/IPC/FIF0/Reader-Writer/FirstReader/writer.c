#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd;
    int value = 42;           // shared resource
    char *fifo = "rw_fifo";

    mkfifo(fifo, 0666);

    // Writer writes after some delay (simulate writing)
    sleep(2);
    fd = open(fifo, O_WRONLY);
    write(fd, &value, sizeof(value));
    close(fd);

    return 0;
}