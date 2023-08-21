#include <stdio.h>
#include <stdlib.h>

#define MAX_COLOR 255
#define BRIGHTNESS 25
#define CHUNK_SIZE 1024 // define size of the chunks to read and write
#define THRESHOLD 128   // define threshold value for the brightness condition

int bright_filter(const char *inputFile, const char *outputFile)
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

    for (int i = 0; i < 54; i++)
    {
        headerInfo[i] = getc(fileIn);
    }

    fwrite(headerInfo, sizeof(unsigned char), 54, fileOut);

    // Extract.. of image from header info
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

    // Read & write image data in chunks until the end of file is reached
    while (!feof(fileIn))
    {

        // Read a chunk of image data from the input file
        size_t bytesRead = fread(chunkBuffer, sizeof(unsigned char), CHUNK_SIZE, fileIn);

        // Apply brightness factor to each pixel in the chunk
        for (int i = 0; i < bytesRead; i++)
        {
            chunkBuffer[i] = chunkBuffer[i] + BRIGHTNESS;
            chunkBuffer[i] = (chunkBuffer[i] > THRESHOLD) ? MAX_COLOR : chunkBuffer[i];
        }

        // Write thresholded image data to output file
        fwrite(chunkBuffer, sizeof(unsigned char), bytesRead, fileOut);
    }

    // Write thresholded image data to output file
    fwrite(chunkBuffer, sizeof(unsigned char), pixelsInImage, fileOut);

    fclose(fileIn);
    fclose(fileOut);

    return 0;
}