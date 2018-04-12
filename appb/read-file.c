#include <stdlib.h>
#include <stdio.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

char *read_file(const char *filename, size_t *length)
{
    int fd;
    struct stat file_info;
    char *buffer;

    fd = open(filename, O_RDONLY);
    fstat(fd, &file_info);
    *length = file_info.st_size;
    if (!S_ISREG(file_info.st_mode)) {
        close(fd);
        return NULL;
    }

    buffer = (char *) malloc(*length);
    read(fd, buffer, *length);
    close(fd);

    return buffer;
}

int main(int argc, char **argv)
{
    char *filename = argv[1];
    size_t length;
    char *buffer = read_file(filename, &length);

    printf("Size of %s: %d B\n", filename, length);
    printf("File contents:\n\n%s\n", buffer);

    free(buffer);

    return 0;
}
