#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    mode_t mode = S_IRWXU | S_IRGRP | S_IWGRP | S_IROTH;
    char *path = argv[1];
    char *filename = argv[2];
    char concat[128];
    char cwd[PATH_MAX];
    int fd;
    ssize_t bytes_read;

    getcwd(cwd, PATH_MAX);
    printf("You're on path %s\n", cwd);

    umask(0);
    printf("Creating directory at %s\n", path);
    while (mkdir(path, mode) < 0) {
        perror("mkdir");
        switch (errno) {
            case EEXIST:
                printf("Deleting the existing directory and trying again.\n");
                rmdir(path);
                break;
            default:
                fprintf(stderr, "Unexpected error.\nAborting!\n");
                exit(1);
                break;
        }
    }

    sprintf(concat, "%s/%s", path, filename);
    printf("Creating file at %s\n", concat);

    if ((fd = open(concat, O_CREAT | O_EXCL | O_WRONLY, mode)) < 0) {
        perror("open");
        exit(1);
    }

    printf("Writing \"%s\" to file %s\n", concat, concat);
    write(fd, concat, strlen(concat));
    close(fd);

    printf("File created at %s\n", concat);

    printf("Opening file at %s for reading\n", concat);
    if ((fd = open(concat, O_RDONLY)) < 0) {
        perror("open");
        exit(1);
    }

    bytes_read = read(fd, cwd, PATH_MAX);

    if (bytes_read == -1) {
        perror("read");
        close(fd);
        exit(1);
    }

    if (bytes_read < PATH_MAX) {
        printf("Read less bytes than expected\n");
        printf("(expeced %d but got %d)\n", PATH_MAX, bytes_read);
        cwd[bytes_read] = '\0';
    }

    printf("File contents:\n%s\n", cwd);
    close(fd);

    printf("Deleting file %s\n", concat);
    unlink(concat);
    printf("Renaming directory %s to lombra.d\n", path);
    rename(path, "lombra.d");

    return 0;
}
