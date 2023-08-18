#include <stdio.h>
#define MAX_COLOR 255
#define THRESHOLD 40 // define threshold value for darkness
#define CHUNK_SIZE 1024 // define size of chunks to read and write

int dark_filter(inputFile, outputFile) {
    
    FILE *fileIn = fopen(inputFile, "rb");
    FILE *fileOut = fopen(outputFile, "wb+");
    
    if (fileIn == NULL || fileOut == NULL) {
        printf("File does not exist.\n");
        if (fileIn != NULL) fclose(fileIn);
        if (fileOut != NULL) fclose(fileOut);
        return 1;
    }

    int i;
    unsigned char byte[54]; // store header info of image
    unsigned char colorTable[1024]; // store color table of image

    // read header information of image
    for(i = 0; i < 54; i++) {
        byte[i] = getc(fileIn);
    }

    // write header info to output file
    fwrite(byte, sizeof(unsigned char), 54, fileOut);

    // extract height, width and bitDepth of the image from header information
    int height = *(int*)&byte[18];
    int width = *(int*)&byte[22];
    int bitDepth = *(int*)&byte[28];

    // calculate size of image in pixels
    int size = height * width;

    // check if image has a color table
    if(bitDepth <= 8) {
        // read, then write the color table from input file
        fread(colorTable, sizeof(unsigned char), 1024, fileIn);
        fwrite(colorTable, sizeof(unsigned char), 1024, fileOut);
    }

    // array to store image data in chunks
    unsigned char buffer[CHUNK_SIZE];

    // read & write image data in chunks until end of the file reached
    while(!feof(fileIn)) {

        // read a chunk of image data from input file
        size_t bytesRead = fread(buffer, sizeof(unsigned char), CHUNK_SIZE, fileIn);
        
        // apply darkness threshold to each pixel in chunk
        for (i = 0; i < bytesRead; i++) {
            buffer[i] = buffer[i] + THRESHOLD;
            buffer[i] = (buffer[i] > THRESHOLD) ? MAX_COLOR : buffer[i];
        }
        // write thresholded image data to the output file
        fwrite(buffer, sizeof(unsigned char), bytesRead, fileOut);
    }

    // write thresholded image data to the output file
    fwrite(buffer, sizeof(unsigned char), size, fileOut);

    fClose(fileIn);
    fclose(fileOut);
    return 0;
}