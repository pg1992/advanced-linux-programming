#include <stdio.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    char *path = argv[1];
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;

    int fd = open(path, O_WRONLY | O_EXCL | O_CREAT, mode);

    if (fd == -1) {
        perror("open");
        return 1;
    }

    return 0;
}
