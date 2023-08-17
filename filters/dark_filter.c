#include <stdio.h>
#define MAX_COLOR 255  // define the maximum color value
#define THRESHOLD 40 // define the threshold value for darkness
#define CHUNK_SIZE 1024 // define the size of the chunks to read and write

int dark_filter(inputFile, outputFile) {
    FILE *fileIn = fopen(inputFile, "rb");  // open the input file
    FILE *fileOut = fopen(outputFile, "wb+"); // create the output file
    int i; // variable to iterate
    unsigned char byte[54]; // array to store the header information of the image
    unsigned char colorTable[1024]; // array to store the color table of the image
    // check if the input file exists
    if(fileIn == NULL) {
        printf("File does not exist.\n");
        return 1;
    }
    // read the header information of the image
    for(i = 0; i < 54; i++) {
        byte[i] = getc(fileIn);
    }
    // write the header information to the output file
    fwrite(byte, sizeof(unsigned char), 54, fileOut);
    // extract the height, width and bitDepth of the image from the header information
    int height = *(int*)&byte[18];
    int width = *(int*)&byte[22];
    int bitDepth = *(int*)&byte[28];
    // calculate the size of the image in pixels
    int size = height * width;
    // check if the image has a color table
    if(bitDepth <= 8) {
        // read, and then write the color table from the input file
        fread(colorTable, sizeof(unsigned char), 1024, fileIn);
        fwrite(colorTable, sizeof(unsigned char), 1024, fileOut);
    }
    // array to store the image data in chunks
    unsigned char buffer[CHUNK_SIZE];
    // read and write the image data in chunks until the end of the file is reached
    while(!feof(fileIn)) {
        // read a chunk of image data from the input file
        size_t bytesRead = fread(buffer, sizeof(unsigned char), CHUNK_SIZE, fileIn);
        // apply the darkness threshold to each pixel in the chunk
        for (i = 0; i < bytesRead; i++) {
            buffer[i] = buffer[i] + THRESHOLD;
            buffer[i] = (buffer[i] > THRESHOLD) ? MAX_COLOR : buffer[i];
        }
        // write the thresholded image data to the output file
        fwrite(buffer, sizeof(unsigned char), bytesRead, fileOut);
    }
    // write the thresholded image data to the output file
    fwrite(buffer, sizeof(unsigned char), size, fileOut);
    // close the input and output files
    fClose(fileIn);
    fclose(fileOut);
    // exit
    return 0;
}