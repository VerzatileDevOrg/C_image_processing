#include <stdio.h>
#define MAX_VALUE 255 // max pixel value

int sepia_filter(inputFile, outputFile) {
    FILE *fileIn = fopen(inputFile, "rb");  // open the input file
    FILE *fileOut = fopen(outputFile, "wb+"); // create the output file
    int i, r, g, b;
    unsigned char byte[54];
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


    unsigned char (*buffer)[3] = malloc(size * sizeof(*buffer));// store the image data
    
    // Flag to check if memory allocation was successful
    if (buffer == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    
    for(i = 0; i < size; i++) {
        r = 0;
        g = 0;
        b = 0;
        // red
        buffer[i][0] = getc(fileIn);
        // green
        buffer[i][1] = getc(fileIn);
        // blue
        buffer[i][2] = getc(fileIn);
        // check to see if each pixel value is above 255
        if(r > MAX_VALUE) {
            r = MAX_VALUE;
        } else if (g > MAX_VALUE) {
            g = MAX_VALUE;
        } else if (b > MAX_VALUE) {
            b = MAX_VALUE;
        }
        // each pixel has three color channels, write the gray value three times
        putc(b, fileOut);
        putc(b, fileOut);
        putc(b, fileOut);
    }
    // close the input and output files
    fClose(fileIn);
    fclose(fileOut);
    // exit
    return 0;
}