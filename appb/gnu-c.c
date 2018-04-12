#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Size of each buffer. */
#define BUFF_SIZE 20
/* Size of struct iovec array. */
#define VEC_SIZE 20

int main(int argc, char **argv)
{
    /* The first argument is the name of the file. */
    char *filename = argv[1];
    /* Open the file using the Standard C Library I/O functions. */
    FILE *fp = fopen(filename, "r");
    /* Get the file descriptor from the file stream. */
    int fd = fileno(fp);
    struct iovec *vec, *vec_next;
    /* Create an array of buffers. */
    char buffer[VEC_SIZE][BUFF_SIZE + 1];
    int i;

    /* Specify each buffer address and size inside of an array of
     * struct iovec. */
    vec = (struct iovec *) malloc(VEC_SIZE * sizeof(struct iovec));
    for (i = 0, vec_next = vec; i < VEC_SIZE; i++, vec_next++) {
        vec_next->iov_base = buffer[i];
        vec_next->iov_len = BUFF_SIZE;
        /* End each buffer with a NUL char so that it prints nicely. */
        buffer[i][BUFF_SIZE] = '\0';
    }

    /* Read the contents of the file into the vector. */
    readv(fd, vec, VEC_SIZE);
    close(fd);

    /* Print each buffer. */
    for (i = 0; i < VEC_SIZE; i++)
        printf("*** %s ***\n", buffer[i]);

    return 0;
}
