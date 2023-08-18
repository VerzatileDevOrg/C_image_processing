#include <stdio.h>

int rgb_to_gray_filter(inputFile, outputFile) {

    FILE *fileIn = fopen(inputFile, "rb");
    FILE *fileOut = fopen(outputFile, "wb+");
    
    int i;
    unsigned char byte[54]; // store header info

    if (fileIn == NULL || fileOut == NULL) {
        printf("File does not exist.\n");
        if (fileIn != NULL) fclose(fileIn);
        if (fileOut != NULL) fclose(fileOut);
        return 1;
    }

    // read header info of image
    for(i = 0; i < 54; i++) {
        byte[i] = getc(fileIn);
    }

    // write header info to output file
    fwrite(byte, sizeof(unsigned char), 54, fileOut);

    // extract height, width & bitDepth of image from header info
    int height = *(int*)&byte[18];
    int width = *(int*)&byte[22];
    int bitDepth = *(int*)&byte[28];
    int size = height * width;

    unsigned char (*buffer)[3] = malloc(size * sizeof(*buffer)); // store image data
    if (buffer == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    
    unsigned char y;
    for(i = 0; i < size; i ++) {
        y = 0;
        buffer[i][0] = getc(fileIn); // red
        buffer[i][1] = getc(fileIn); // green
        buffer[i][2] = getc(fileIn); // blue

        // RGB to gray
        y = (buffer[i][0] * 0.3) + (buffer[i][2] * 0.11);

        // Triplicate grayscale value for three color channels
        putc(y, fileOut);
        putc(y, fileOut);
        putc(y, fileOut);
    }

    fClose(fileIn);
    fclose(fileOut);
    return 0;
}