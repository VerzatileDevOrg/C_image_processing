#include <stdio.h>

#define MAX_COLOR 255

int emboss_filter(const char *inputFile, const char *outputFile)
{
    FILE *fileIn = fopen(inputFile, "rb");
    FILE *fileOut = fopen(outputFile, "wb+");
    unsigned char byte[54];
    int i, j, r, g, b, gray, prev_gray, diff;

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
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            // read pixel color
            b = getc(fileIn);
            g = getc(fileIn);
            r = getc(fileIn);

            // convert to grayscale
            gray = 0.299 * r + 0.587 * g + 0.114 * b;

            if (i > 0 && j > 0)
            {
                // calculate difference between current and previous pixel grayscale values
                diff = gray - prev_gray;

                // calculate new pixel color
                r = g = b = MAX_COLOR / 2 + diff;

                // clamp color values to [0, MAX_COLOR]
                r = fmax(0, fmin(r, MAX_COLOR));
                g = fmax(0, fmin(g, MAX_COLOR));
                b = fmax(0, fmin(b, MAX_COLOR));
            }

            // set current grayscale value as previous grayscale value for next iteration
            prev_gray = gray;

            // set color of current pixel
            buffer[i * width + j][0] = r;
            buffer[i * width + j][1] = g;
            buffer[i * width + j][2] = b;
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