// writer.c - writes a message to /dev/msgdriver
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    const char *path = "/dev/msgdriver";
    int fd;
    ssize_t ret;
    char *msg;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s \"message text\"\n", argv[0]);
        return 1;
    }

    msg = argv[1];

    fd = open(path, O_WRONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    ret = write(fd, msg, strlen(msg));
    if (ret < 0) {
        perror("write");
        close(fd);
        return 1;
    }

    printf("Wrote %zd bytes to %s\n", ret, path);

    close(fd);
    return 0;
}
