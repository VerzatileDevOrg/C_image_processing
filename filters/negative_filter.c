#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int negative_filter(const char *inputFile, const char *outputFile) {
    FILE *fileIn = fopen(inputFile, "rb"); // open the input file for reading in binary mode
    FILE *fileOut = fopen(outputFile, "wb+"); // create the output file

    unsigned char *imageData;
    unsigned char *newImageData;
    unsigned char imageHeader[54];
    unsigned char colorTable[1024];

    int i, j;
    
    // check if the input file exists
    fread(imageHeader, sizeof(unsigned char), 54, fileIn);
    int width = *(int*)&imageHeader[18];
    int height = *(int*)&imageHeader[22];
    int bitDepth = *(int*)&imageHeader[28];
    int imageDataSize = width * height;
    
    imageData = (unsigned char*)malloc(imageDataSize * sizeof(unsigned char));
    newImageData = (unsigned char*)malloc(imageDataSize * sizeof(unsigned char));
    
    // check if the image has a color table
    if (bitDepth <= 8) {
        fread(colorTable, sizeof(unsigned char), 1024, fileIn);
    }
    
    fread(imageData, sizeof(unsigned char), imageDataSize, fileIn);

    // apply the negative filter to each pixel
    unsigned char *p = imageData;
    unsigned char *q = newImageData;
    for (i = 0; i < height * width; i++) {
        *q++ = 255 - *p++;
    }

    // write the image data to the output file
    fwrite(imageHeader, sizeof(unsigned char), 54, fileOut);
    if (bitDepth <= 8) {
        fwrite(colorTable, sizeof(unsigned char), 1024, fileOut);
    }
    
    fwrite(newImageData, sizeof(unsigned char), imageDataSize, fileOut);

    fclose(fileIn);
    fclose(fileOut);
    
    // free the memory allocated for the image data
    free(imageData);
    free(newImageData);
    
    return 0;
}