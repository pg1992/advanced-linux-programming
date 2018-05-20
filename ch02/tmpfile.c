#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    /* Use TMPFILE to create a temporary file stream that will be deleted
     * after the it is closed of the program finishes. */
    FILE *fp = tmpfile();
    char *input, *output;
    size_t length;

    /* Input is the first argument or some pre-defined text if there
     * was no arguments. */
    if (argc > 1)
        input = argv[1];
    else
        input = "Goodbye, world...";

    printf("input  = %s\n", input);

    /* The length will be the first byte written to the file. */
    length = strlen(input);
    printf("length = %d\n", length);

    /* Write the length of the message first. */
    fwrite(&length, sizeof(length), 1, fp);
    /* Write the message to the file.. */
    fwrite(input, length, 1, fp);

    /* Rewind to the beginning of the temporary file. */
    fseek(fp, 0L, SEEK_SET);

    /* Read the length of the message. */
    fread(&length, sizeof(length), 1, fp);
    /* Allocate the neccessary amount of bytes to the output buffer. */
    output = (char *)malloc(length);
    /* Read the contents of the message on the temporary file into the
     * output buffer. */
    fread(output, length, 1, fp);

    printf("output = %s\n", output);
    printf("length = %d\n", length);

    /* Close the temporary file.  We don't need to unlink it first. */
    fclose(fp);

    return 0;
}
