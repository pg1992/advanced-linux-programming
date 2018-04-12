#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int zero = 0;
    const int megabyte = 1024 * 1024;
    char *filename = argv[1];
    size_t length = (size_t) atoi(argv[2]) * megabyte;

    int fd = open(filename, O_CREAT | O_EXCL | O_WRONLY, 0666);
    lseek(fd, length - 1, SEEK_SET);
    write(fd, &zero, 1);
    close(fd);

    return 0;
}
