#include <stdlib.h>
#include <stdio.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/* Read the contents of FILENAME into a newly allocated buffer.  The
 * size of the buffer is stored in *LENGTH.  Returns the buffer, which
 * the callser must free.  If FILENAME doesn't correspond to a regular
 * file, returns NULL. */

char *read_file(const char *filename, size_t *length)
{
    int fd;
    struct stat file_info;
    char *buffer;

    /* Opent the file. */
    fd = open(filename, O_RDONLY);

    /* Get information about the file. */
    fstat(fd, &file_info);
    *length = file_info.st_size;
    /* Make sure the file is an ordinary file. */
    if (!S_ISREG(file_info.st_mode)) {
        /* It's not, so give up. */
        close(fd);
        return NULL;
    }

    /* Allocate a buffer large enough to hold the file's contents. */
    buffer = (char *) malloc(*length);
    /* Read the file into the buffer. */
    read(fd, buffer, *length);

    /* Finish up. */
    close(fd);
    return buffer;
}

int main(int argc, char **argv)
{
    /* The filename is the first argument passed. */
    char *filename = argv[1];
    size_t length;
    /* We have the new buffer and must `free` it at the end. */
    char *buffer = read_file(filename, &length);

    /* Print file name, size, and contents. */
    printf("Size of %s: %d B\n", filename, length);
    printf("File contents:\n\n%s\n", buffer);

    /* All done. */
    free(buffer);
    return 0;
}
