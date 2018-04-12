#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFF_SIZE 20
#define VEC_SIZE 20

int main(int argc, char **argv)
{
    char *filename = argv[1];
    FILE *fp = fopen(filename, "r");
    int fd = fileno(fp);
    struct iovec *vec, *vec_next;
    char buffer[VEC_SIZE][BUFF_SIZE + 1];
    int i;

    vec = (struct iovec *) malloc(VEC_SIZE * sizeof(struct iovec));
    for (i = 0, vec_next = vec; i < VEC_SIZE; i++, vec_next++) {
        vec_next->iov_base = buffer[i];
        vec_next->iov_len = BUFF_SIZE;
        buffer[i][BUFF_SIZE] = 0;
    }

    readv(fd, vec, VEC_SIZE);
    close(fd);

    for (i = 0; i < VEC_SIZE; i++)
        printf("*** %s ***\n", buffer[i]);

    return 0;
}
