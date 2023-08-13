#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int negative_filter(inputFile, outputFile) {
    FILE* fileIn= fopen(inputFile, "rb"); // open the input file for reading in binary mode
    File *fileOut = fopen(outputFile, "wb+"); // create the output file
    // declare variables to store the image data and header information
    unsigned char *imageData;
    unsigned char *newImageData;
    unsigned char imageHeader[54];
    unsigned char colorTable[1024];
    // declare loop variables
    int i, j;
    // read the header information of the input image file
    fread(imageHeader, sizeof(unsigned char), 54, fileIn);
    // extract the height, width and bitDepth of the image from the header information
    int width = *(int*)&imageHeader[18];
    int height = *(int*)&imageHeader[22];
    int bitDepth = *(int*)&imageHeader[28];
    // calculate the size of the new image data in bytes
    int imageDataSize = width * height;
    // allocate memory to store the pixel data of the input and output images
    imageData = (unsigned char*)malloc (imageDataSize * sizeof(unsignedchar));
    newImageData = (unsigned char*)malloc (imageDataSize * sizeof(unsignedchar));
    // check if the image has a color table
    if(bitDepth <= 8) {
        fread(colorTable, sizeof(unsigned char), 1024, fileIn);
    }
    // read the pixel data of the input image from the input file
    fread(imageData, sizeof(unsigned char), imageDataSize, fileIn);

    // apply the negative filter to each pixel of the image
    unsigned char *p = imageData;
    unsigned char *q = newImageData;
    for(i = 0; i < height * width; i++) {
        *q++ = 255 - *p++;
    }
    // open the output image file in binary mode.
    FILE *fileOut = fopen(outputFile, "wb");
    // write the header information of the output image to the output file
    fwrite(imageHeader, sizeof(unsigned char), 54, fileOut);
    // if the image has a color table, write it to the output file
    if(bitDepth <= 8) {
        fwrite(colorTable, sizeof(unsigned char), 1024, fileOut);
    }
    // write the pixel data of the negative image to the output file
    fwrite(newImageData, sizeof(unsigned char), imageDataSize, fileOut);
    // close the input and output files
    fclose(fileIn);
    fclose(fileOut);
    // exit
    return 0;
}