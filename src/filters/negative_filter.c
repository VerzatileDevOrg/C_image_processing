#include <stdio.h>

int negative_filter(const char *inputFile, const char *outputFile)
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

    unsigned char *imageData = NULL;
    unsigned char *newImageData = NULL;
    unsigned char headerInfo[54];
    unsigned char colorTable[1024];

    // Read image header
    fread(headerInfo, sizeof(unsigned char), 54, fileIn);
    int width = *(int *)&headerInfo[18];
    int height = *(int *)&headerInfo[22];
    int bitDepth = *(int *)&headerInfo[28];
    int imageDataSize = width * height * bitDepth;

    // Allocate memory for image data
    imageData = (unsigned char *)malloc(imageDataSize * sizeof(unsigned char));
    newImageData = (unsigned char *)malloc(imageDataSize * sizeof(unsigned char));

    // Read color table if present
    if (bitDepth <= 8)
    {
        fread(colorTable, sizeof(unsigned char), 1024, fileIn);
    }

    // Read original image data
    fread(imageData, sizeof(unsigned char), imageDataSize, fileIn);

    // Apply negative filter to each pixel
    unsigned char *p = imageData;
    unsigned char *q = newImageData;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int index = (y * width + x) * (bitDepth / 8);

            for (int channel = 0; channel < bitDepth / 8; channel++)
            {
                newImageData[index + channel] = 255 - imageData[index + channel];
            }
        }
    }
    // Write image data to output file
    fwrite(headerInfo, sizeof(unsigned char), 54, fileOut);

    if (bitDepth <= 8)
    {
        fwrite(colorTable, sizeof(unsigned char), 1024, fileOut);
    }

    fwrite(newImageData, sizeof(unsigned char), imageDataSize, fileOut);

    // Clean up and close files
    fclose(fileIn);
    fclose(fileOut);

    free(imageData);
    free(newImageData);

    return 0;
}