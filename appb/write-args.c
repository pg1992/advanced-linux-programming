#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    int fd;
    struct iovec *vec;
    struct iovec *vec_next;
    int i;
    char newline = '\n';
    char *filename = argv[1];
    argc -= 2;
    argv += 2;

    vec = (struct iovec *) malloc(2 * argc * sizeof(struct iovec));
    vec_next = vec;
    for (i = 0; i < argc; i++) {
        vec_next->iov_base = argv[i];
        vec_next->iov_len = strlen(argv[i]);
        vec_next++;
        vec_next->iov_base = &newline;
        vec_next->iov_len = 1;
        vec_next++;
    }

    fd = open(filename, O_WRONLY | O_CREAT);
    writev(fd, vec, 2 * argc);
    close(fd);
    free(vec);

    return 0;
}
