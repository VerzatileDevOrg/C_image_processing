#include <stdio.h>

#define MAX_VALUE 255

int sepia_filter(const char *inputFile, const char *outputFile)
{
    FILE *fileIn = fopen(inputFile, "rb");
    FILE *fileOut = fopen(outputFile, "wb+");
    unsigned char byte[54];
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
        byte[i] = getc(fileIn);
    }

    // write header info to output file
    fwrite(byte, sizeof(unsigned char), 54, fileOut);
    // extract attributes from image header
    int height = *(int *)&byte[18];
    int width = *(int *)&byte[22];
    int bitDepth = *(int *)&byte[28];
    int size = height * width;

    // check if image has a color table
    unsigned char buffer[size][3];

    // read & write image data in chunks until end of file reached
    for (i = 0; i < size; i++)
    {
        r = 0;
        g = 0;
        b = 0;

        buffer[i][2] = getc(fileIn); // blue
        buffer[i][1] = getc(fileIn); // green
        buffer[i][0] = getc(fileIn); // red

        // conversion formula of rgb to sepia
        r = (buffer[i][0] * 0.393) + (buffer[i][1] * 0.769) + (buffer[i][2] * 0.189);
        g = (buffer[i][0] * 0.349) + (buffer[i][1] * 0.686) + (buffer[i][2] * 0.168);
        b = (buffer[i][0] * 0.272) + (buffer[i][1] * 0.534) + (buffer[i][2] * 0.131);

        // check if value is greater than 255
        if (r > MAX_VALUE)
        {
            r = MAX_VALUE;
        }

        if (g > MAX_VALUE)
        {
            g = MAX_VALUE;
        }

        if (b > MAX_VALUE)
        {
            b = MAX_VALUE;
        }

        // write sepia image data to output file
        putc(b, fileOut);
        putc(g, fileOut);
        putc(r, fileOut);
    }

    fclose(fileOut);
    fclose(fileIn);

    return 0;
}