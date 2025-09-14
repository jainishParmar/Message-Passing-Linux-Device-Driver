// reader.c - reads a message from /dev/msgdriver
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define BUF_SZ 1024

int main(void)
{
    const char *path = "/dev/msgdriver";
    int fd;
    ssize_t ret;
    char buf[BUF_SZ];

    fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    ret = read(fd, buf, sizeof(buf) - 1);
    if (ret < 0) {
        perror("read");
        close(fd);
        return 1;
    }

    buf[ret] = '\0';
    printf("Read %zd bytes: '%s'\n", ret, buf);

    close(fd);
    return 0;
}
