#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    FILE *fp = tmpfile();
    char *input, *output;
    size_t length;

    if (argc > 1)
        input = argv[1];
    else
        input = "Goodbye, world...";

    length = strlen(input);
    printf("length = %d\n", length);

    fwrite(&length, sizeof(length), 1, fp);
    fwrite(input, length, 1, fp);

    fseek(fp, 0L, SEEK_SET);

    fread(&length, sizeof(length), 1, fp);
    output = (char *)malloc(length);
    fread(output, length, 1, fp);

    printf("output = %s\n", output);
    printf("length = %d\n", length);

    fclose(fp);

    return 0;
}
