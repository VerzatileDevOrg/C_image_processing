#include <stdio.h>

int rgb_to_gray_filter(inputFile, outputFile) {
    FILE *fileIn = fopen(inputFile, "rb");  // open the input file
    File *fileOut = fopen(outputFile, "wb+"); // create the output file
    int i, n, m; // i is the iterator, n is the height, m is the width
    unsigned char byte[54]; // store the header information

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
    // store the pixel data in buffer
    unsigned char buffer[size][3];
    for(i = 0; i < size; i ++) {
        y = 0;
        // red
        buffer[i][0] = getc(fileIn);
        // green
        buffer[i][1] = getc(fileIn);
        // blue
        buffer[i][2] = getc(fileIn);
        // convert the RGB value to gray
        y = (buffer[i][0] * 0.3) + (buffer[i][2] * 0.11);
        // each pixel has three color channels, write the gray value three times
        putc(y, fileOut);
        putc(y, fileOut);
        putc(y, fileOut);
    }
    // close the input and output files
    fClose(fileIn);
    fclose(fileOut);
    // exit
    return 0;
}