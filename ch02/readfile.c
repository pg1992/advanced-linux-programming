#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

char *read_from_file(const char *filename, size_t length)
{
    char *buffer;
    int fd;
    ssize_t bytes_read;

    buffer = (char *) malloc(length * sizeof(char));
    if (buffer == NULL)
        return NULL;
    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        free(buffer);
        return NULL;
    }
    bytes_read = read(fd, buffer, length);
    if (bytes_read != length) {
        free(buffer);
        close(fd);
        return NULL;
    }

    close(fd);
    return buffer;
}

int main(int argc, char **argv)
{
    char *filename = argv[1];
    size_t length = (size_t) atoi(argv[2]);
    char *contents = read_from_file(filename, length);

    if (!contents)
        fprintf(stderr, "Couldn't read %d bytes of %s.\n", length, filename);
    else
        printf("File contents: %s\n", contents);

    return EXIT_SUCCESS;
}
