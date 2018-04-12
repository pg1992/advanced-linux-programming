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

    /* Open a new file. */
    int fd = open(filename, O_CREAT | O_EXCL | O_WRONLY, 0666);
    /* Jump to 1 byte short of where we want the file to end. */
    lseek(fd, length - 1, SEEK_SET);
    /* Write a single 0 byte. */
    write(fd, &zero, 1);
    /* All done. */
    close(fd);

    return 0;
}
