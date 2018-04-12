#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>

ssize_t write_all(int fd, const void *buffer, size_t count) {
    size_t left_to_write = count;
    while (left_to_write > 0) {
        size_t written = write(fd, buffer, left_to_write);
        if (written == -1) {
            return -1;
        } else {
            left_to_write -= written;
            buffer += written;
        }
    }
    assert(left_to_write == 0);
    return count;
}

int main(int argc, char **argv)
{
    char *filename = argv[1];
    int fd = open(filename, O_CREAT | O_EXCL | O_WRONLY);
    char *buffer = argv[2];
    ssize_t written = write_all(fd, buffer, strlen(buffer));

    if (written < 0)
        perror("write_all");
    else
        printf("Successfully writen %d bytes to %s\n", written, filename);

    return 0;
}
