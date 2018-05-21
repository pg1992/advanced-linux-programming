#include <stdio.h>
#include <tiffio.h> // this depends on libz and libjpeg (run `ldd tifftest`)

int main(int argc, char **argv)
{
    TIFF* tiff;
    tiff = TIFFOpen(argv[1], "r");
    TIFFClose(tiff);

    return 0;
}
