#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd;
    int value = 99;           // shared resource
    char *fifo = "rw_fifo2";

    mkfifo(fifo, 0666);

    // Writer wants priority: sleeps less to "win" over readers
    sleep(1);
    fd = open(fifo, O_WRONLY);
    write(fd, &value, sizeof(value));
    close(fd);

    return 0;
}