/*
    readjpg example
    converts input jpeg image to PPM and writes the data to stdout

    gcc main.c -Wall -Werror
    ./a.out /path/to/file.jpg | display

    should display the jpeg file (as ppm)
*/

#include <stdio.h>
#include <stdint.h>

#include "readjpg.c"

typedef uint_least8_t u8;

static
int jpg2ppm(FILE* src, FILE* dst)
{
    Rawimage** imgs;
    Rawimage* img;
    size_t i;

    imgs = readjpg(src, CRGB);
    fputs("Done!\n", stderr);

    if (!*imgs) {
        fprintf(stderr, "Decoder returned no images\n");
        return 1;
    }

    img = *imgs;

    if (img->nchans != 4 && img->nchans != 3)
    {
        fprintf(stderr, "Unsupported nchans: %u\n", img->nchans);
        return 1;
    }

    if (img->nchans == 4)
    {
        fprintf(stderr, "Warning: alpha will be stripped\n");
    }

    fprintf(dst, "P6 %u %u 255\n", img->r.max.x, img->r.max.y);

    for (i = 0; i < img->r.max.x * img->r.max.y; ++i)
    {
        u8 r = img->chans[0][i];
        u8 g = img->chans[1][i];
        u8 b = img->chans[2][i];

        fputc((char)r, dst);
        fputc((char)g, dst);
        fputc((char)b, dst);
    }

    return 0;
}

int main(int argc, char* argv[])
{
    int res = 0;
    FILE* f;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }

    fprintf(stderr, "Decoding %s...\n", argv[1]);

    f = fopen(argv[1], "rb");
    if (!f) {
        perror("fopen");
        return 1;
    }

    res = jpg2ppm(f, stdout);

    fclose(f);

    return res;
}
