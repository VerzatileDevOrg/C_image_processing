#include <stdio.h>

#define THRESHOLD 128 // define value of threshold for black and white
#define WHITE 255
#define BLACK 0
#define CHUNK_SIZE 1024 // define size of chunks to read and write

int black_and_white_filter(const char *inputFile, const char *outputFile)
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

    // Extract.. of image from the header information
    int height = *(int *)&headerInfo[18];
    int width = *(int *)&headerInfo[22];
    int bitDepth = *(int *)&headerInfo[28];
    int size = height * width;

    // Check if the image has a color table
    if (bitDepth <= 8)
    {
        fread(colorTable, sizeof(unsigned char), 1024, fileIn);
        fwrite(colorTable, sizeof(unsigned char), 1024, fileOut);
    }

    unsigned char chunkBuffer[CHUNK_SIZE];

    // Read & write the image data in chunks until the end of file is reached
    while (!feof(fileIn))
    {

        // Read a chunk of image data from input file
        size_t bytesRead = fread(chunkBuffer, sizeof(unsigned char), CHUNK_SIZE, fileIn);

        // Apply threshold to each pixel in the chunk
        for (int i = 0; i < bytesRead; i++)
        {
            chunkBuffer[i] = (chunkBuffer[i] > THRESHOLD)
                                 ? WHITE
                                 : BLACK;
        }
        // Write the thresholded image data to the output file
        fwrite(chunkBuffer, sizeof(unsigned char), bytesRead, fileOut);
    }

    fclose(fileIn);
    fclose(fileOut);

    return 0;
}