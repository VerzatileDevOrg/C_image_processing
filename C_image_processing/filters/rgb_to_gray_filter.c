#include <stdio.h>

int rgb_to_gray_filter(inputFile, outputFile) {

    FILE *fileIn = fopen(inputFile, "rb");
    FILE *fileOut = fopen(outputFile, "wb+");

    if (fileIn == NULL || fileOut == NULL) {
        printf("File does not exist.\n");
        if (fileIn != NULL) fclose(fileIn);
        if (fileOut != NULL) fclose(fileOut);
        return 1;
    }

    // Read and write header info
    unsigned char headerInfo[54];
    fread(headerInfo, sizeof(unsigned char), 54, fileIn);
    fwrite(headerInfo, sizeof(unsigned char), 54, fileOut);

    // Extract image dimensions from header info
    int height = *(int*)&headerInfo[18];
    int width = *(int*)&headerInfo[22];
    int bitDepth = *(int*)&headerInfo[28];
    int pixelsInImage = height * width;

    // Allocate Memory to Image Data
    unsigned char (*buffer)[3] = malloc(pixelsInImage * sizeof(*buffer));
    if (buffer == NULL) {
        printf("Memory allocation failed.\n");
        fclose(fileIn);
        fclose(fileOut);
        return 1;
    }
    
    for (int i = 0; i < pixelsInImage; i++) {

        // read RGB color components of current pixel from input file
        buffer[i][0] = getc(fileIn); // Red
        buffer[i][1] = getc(fileIn); // Green
        buffer[i][2] = getc(fileIn); // Blue

        // calculate grayscale value using weighted average of red and blue components
        unsigned char grayscaleValue = (buffer[i][0] * 0.3) + (buffer[i][2] * 0.11);
        
        // write the calculated grayscale value to the output file for each RGB channel
        putc(grayscaleValue, fileOut); // Red
        putc(grayscaleValue, fileOut); // Green
        putc(grayscaleValue, fileOut); // Blue
    }

    // Clean up / Close
    fClose(fileIn);
    fclose(fileOut);
    return 0;
}