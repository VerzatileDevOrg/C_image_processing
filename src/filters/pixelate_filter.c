#include <stdio.h>
#include <math.h>

#define MAX_COLOR 255
#define PIXEL_SIZE 10

int pixelate_filter(const char *inputFile, const char *outputFile)
{
    FILE *fileIn = fopen(inputFile, "rb");
    FILE *fileOut = fopen(outputFile, "wb+");
    unsigned char byte[54];
    int i, j, r, g, b;

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
    int size = height * width;

    unsigned char buffer[size][3];

    // read & write image data in chunks until end of file is reached
    for (i = 0; i < height; i += PIXEL_SIZE)
    {
        for (j = 0; j < width; j += PIXEL_SIZE)
        {
            r = 0;
            g = 0;
            b = 0;

            // calculate average color of pixel block
            int count = 0;
            int x, y;
            for (x = i; x < i + PIXEL_SIZE; x++)
            {
                for (y = j; y < j + PIXEL_SIZE; y++)
                {
                    if (x < height && y < width)
                    {
                        r += buffer[x * width + y][0];
                        g += buffer[x * width + y][1];
                        b += buffer[x * width + y][2];
                        count++;
                    }
                }
            }
            r /= count;
            g /= count;
            b /= count;

            // set color of pixel block
            for (x = i; x < i + PIXEL_SIZE; x++)
            {
                for (y = j; y < j + PIXEL_SIZE; y++)
                {
                    if (x < height && y < width)
                    {
                        buffer[x * width + y][0] = r;
                        buffer[x * width + y][1] = g;
                        buffer[x * width + y][2] = b;
                    }
                }
            }
        }
    }

    // write image data to output file
    for (i = 0; i < size; i++)
    {
        putc(buffer[i][2], fileOut); // blue
        putc(buffer[i][1], fileOut); // green
        putc(buffer[i][0], fileOut); // red
    }

    fclose(fileIn);
    fclose(fileOut);

    return 0;
}