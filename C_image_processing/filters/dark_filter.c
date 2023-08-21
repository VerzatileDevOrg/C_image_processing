#include <stdio.h>

#define MAX_COLOR 255
#define DARK_THRESHOLD 40
#define CHUNK_SIZE 1024

int dark_filter(const char *inputFile, const char *outputFile)
{
    FILE *fileIn = fopen(inputFile, "rb");
    FILE *fileOut = fopen(outputFile, "wb+");

    if (fileIn == NULL || fileOut == NULL)
    {
        printf("File does not exist.\n");
        if (fileIn != NULL)
            fclose(fileIn);
        if (fileOut != NULL)
            fclose(fileOut);
        return 1;
    }

    unsigned char headerInfo[54];
    unsigned char colorTable[1024];

    // Read header info of image
    for (int i = 0; i < 54; i++)
    {
        headerInfo[i] = getc(fileIn);
    }

    // Write header info to output file
    fwrite(headerInfo, sizeof(unsigned char), 54, fileOut);

    // Extract.. of the image from header info
    int height = *(int *)&headerInfo[18];
    int width = *(int *)&headerInfo[22];
    int bitDepth = *(int *)&headerInfo[28];
    int pixelsInImage = height * width;

    // Check if image has a color table
    if (bitDepth <= 8)
    {
        fread(colorTable, sizeof(unsigned char), 1024, fileIn);
        fwrite(colorTable, sizeof(unsigned char), 1024, fileOut);
    }

    unsigned char chunkBuffer[CHUNK_SIZE];

    // Read & write image data in chunks until end of file reached
    while (!feof(fileIn))
    {

        // read a chunk of image data from input file
        size_t bytesRead = fread(chunkBuffer, sizeof(unsigned char), CHUNK_SIZE, fileIn);

        // apply darkness threshold to each pixel in chunk
        for (int i = 0; i < bytesRead; i++)
        {
            chunkBuffer[i] = chunkBuffer[i] + DARK_THRESHOLD;
            chunkBuffer[i] = (chunkBuffer[i] > DARK_THRESHOLD) ? MAX_COLOR : chunkBuffer[i];
        }
        // write thresholded image data to the output file
        fwrite(chunkBuffer, sizeof(unsigned char), bytesRead, fileOut);
    }

    // Write thresholded image data to output file
    fwrite(chunkBuffer, sizeof(unsigned char), pixelsInImage, fileOut);

    fclose(fileIn);
    fclose(fileOut);

    return 0;
}