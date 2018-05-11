#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

typedef int temp_file_handle;

temp_file_handle write_temp_file(char *buffer, size_t length)
{
    char temp_filename[] = "/tmp/temp_file.XXXXXX";
    int fd = mkstemp(temp_filename);
    unlink(temp_filename);
    write(fd, &length, sizeof (length));
    write(fd, buffer, length);
    return fd;
}

char *read_temp_file(temp_file_handle temp_file, size_t *length)
{
    char *buffer;
    int fd = temp_file;
    lseek(fd, 0, SEEK_SET);
    read(fd, length, sizeof(*length));
    buffer = (char *) malloc(*length);
    read(fd, buffer, *length);
    close(fd);
    return buffer;
}

int main(int argc, char **argv)
{
    temp_file_handle tf;
    char *input;
    if (argc > 1)
        input = argv[1];
    else
        input = "Hello World!";
    tf = write_temp_file(input, strlen(input));
    char *output;
    size_t l;
    output = read_temp_file(tf, &l);
    printf("%s\n", output);
    return 0;
}
