#include <stdio.h>

#define MAX_VALUE 255

int sepia_filter(inputFile, outputFile) {
    
    FILE *fileIn = fopen(inputFile, "rb");
    FILE *fileOut = fopen(outputFile, "wb+");
    
    if (fileIn == NULL || fileOut == NULL) {
        printf("File does not exist.\n");
        if (fileIn != NULL) fclose(fileIn);
        if (fileOut != NULL) fclose(fileOut);
        return 1;
    }

    // Read header info of image
    unsigned char headerInfo[54];
    for(int i = 0; i < 54; i++) {
        headerInfo[i] = getc(fileIn);
    }
    
    // Write header info to output file
    fwrite(headerInfo, sizeof(unsigned char), 54, fileOut);

    // Extract.. of image from header info
    int height = *(int*)&headerInfo[18];
    int width = *(int*)&headerInfo[22];
    int bitDepth = *(int*)&headerInfo[28];
    int pixelsInImage = height * width;

    // Store image data in buffer
    unsigned char (*buffer)[3] = malloc(pixelsInImage * sizeof(*buffer));
    if (buffer == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    for(int i = 0; i < pixelsInImage; i++) {
        int r = 0, g = 0, b = 0;

        buffer[i][0] = getc(fileIn); // red
        buffer[i][1] = getc(fileIn); // green
        buffer[i][2] = getc(fileIn); // blue
        
        if(r > MAX_VALUE) {
            r = MAX_VALUE;
        } else if (g > MAX_VALUE) {
            g = MAX_VALUE;
        } else if (b > MAX_VALUE) {
            b = MAX_VALUE;
        }
        // add Grayscale filter to each pixel
        putc(b, fileOut);
        putc(b, fileOut);
        putc(b, fileOut);
    }

    fClose(fileIn);
    fclose(fileOut);
    return 0;
}