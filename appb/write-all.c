#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>

/* Write all of COUNT bytes from BUFFER to file descriptor FD.
 * Returns -1 on error, or the number of bytes written. */

ssize_t write_all(int fd, const void *buffer, size_t count) {
    size_t left_to_write = count;
    while (left_to_write > 0) {
        size_t written = write(fd, buffer, left_to_write);
        if (written == -1) {
            /* An error occurred; bail. */
            return -1;
        } else {
            /* Keep count of how much more we need to write. */
            left_to_write -= written;
            buffer += written;
        }
    }
    /* We should have written no more than COUNT bytes! */
    assert(left_to_write == 0);
    /* The number of bytes written is exactly COUNT. */
    return count;
}

int main(int argc, char **argv)
{
    /* The file which will be created and written is the first argument. */
    char *filename = argv[1];
    /* The contents of the file will be the second argument. */
    char *buffer = argv[2];
    /* Create a new file for writing only.  The file should not exist. */
    int fd;
    /* How many bytes were written. */
    ssize_t written;

    if ((fd = open(filename, O_CREAT | O_EXCL | O_WRONLY)) < 0) {
        /* Error opening file. */
        perror("open");
        return -1;
    }

    if ((written = write_all(fd, buffer, strlen(buffer))) < 0) {
        /* Error writing to file. */
        perror("write_all");
        return 1;
    } else {
        printf("Successfully writen %d bytes to %s\n", written, filename);
    }

    /* All done. */
    close(fd);
    return 0;
}
