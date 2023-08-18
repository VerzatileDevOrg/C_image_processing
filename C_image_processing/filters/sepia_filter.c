#include <stdio.h>
#define MAX_VALUE 255

int sepia_filter(inputFile, outputFile) {
    
    FILE *fileIn = fopen(inputFile, "rb");
    FILE *fileOut = fopen(outputFile, "wb+");
    
    int i, r, g, b;
    unsigned char byte[54];
    
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

    // extract height, width and bitDepth of image from header info
    int height = *(int*)&byte[18];
    int width = *(int*)&byte[22];
    int bitDepth = *(int*)&byte[28];

    // calculate size of image in pixels
    int size = height * width;

    unsigned char (*buffer)[3] = malloc(size * sizeof(*buffer)); // store the image data
    if (buffer == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    for(i = 0; i < size; i++) {
        r = 0; g = 0; b = 0;

        buffer[i][0] = getc(fileIn); // red
        buffer[i][1] = getc(fileIn); // green
        buffer[i][2] = getc(fileIn); // blue

        // each pixel value is above 255?
        if(r > MAX_VALUE) {
            r = MAX_VALUE;
        } else if (g > MAX_VALUE) {
            g = MAX_VALUE;
        } else if (b > MAX_VALUE) {
            b = MAX_VALUE;
        }
        // Triplicate grayscale value for three color channels
        putc(b, fileOut);
        putc(b, fileOut);
        putc(b, fileOut);
    }

    fClose(fileIn);
    fclose(fileOut);
    return 0;
}