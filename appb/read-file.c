#include <stdlib.h>

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
    return 0;
}
