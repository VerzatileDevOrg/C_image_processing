#include <stdio.h>

#define MAX_COLOR 255
#define DARK_FACTOR 0.4

int dark_filter(const char *inputFile, const char *outputFile)
{
    FILE *fileIn = fopen(inputFile, "rb");
    FILE *fileOut = fopen(outputFile, "wb+");
    unsigned char headerInfo[54];
    int i, r, g, b;

    if (fileIn == NULL || fileOut == NULL)
    {
        printf("File does not exist.\n");
        if (fileIn != NULL)
            fclose(fileIn);
        if (fileOut != NULL)
            fclose(fileOut);
        return 1;
    }

    // read header info of image
    for (i = 0; i < 54; i++)
    {
        headerInfo[i] = getc(fileIn);
    }

    // write header info to output file
    fwrite(headerInfo, sizeof(unsigned char), 54, fileOut);
    // extract attributes from image header
    int height = *(int *)&headerInfo[18];
    int width = *(int *)&headerInfo[22];
    int size = height * width;

    unsigned char buffer[size][3];

    // read & write image data in chunks until end of file is reached
    for (i = 0; i < size; i++)
    {
        r = 0;
        g = 0;
        b = 0;

        buffer[i][2] = getc(fileIn); // blue
        buffer[i][1] = getc(fileIn); // green
        buffer[i][0] = getc(fileIn); // red

        // apply darkness effect to each channel
        r = buffer[i][0] * DARK_FACTOR;
        g = buffer[i][1] * DARK_FACTOR;
        b = buffer[i][2] * DARK_FACTOR;

        // Write darkened image data to output file
        putc(b, fileOut);
        putc(g, fileOut);
        putc(r, fileOut);
    }

    fclose(fileOut);
    fclose(fileIn);

    return 0;
}