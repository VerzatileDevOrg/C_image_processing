#include <stdio.h>
#define THRESHOLD 128 // define value of threshold for black and white
#define WHITE 255
#define BLACK 0
#define CHUNK_SIZE 1024 // define size of chunks to read and write

int black_and_white_filter(const char *inputFile, const char *outputFile) {
    
    FILE *fileIn = fopen(inputFile, "rb");
    FILE *fileOut = fopen(outputFile, "wb+");

    if (fileIn == NULL || fileOut == NULL) {
        printf("File does not exist.\n");
        if (fileIn != NULL) fclose(fileIn);
        if (fileOut != NULL) fclose(fileOut);
        return 1;
    }

    int i;
    unsigned char byte[54];
    unsigned char colorTable[1024];

    // read header info of image
    for(i = 0; i < 54; i++) {
        byte[i] = getc(fileIn);
    }

    // write header info to output file
    fwrite(byte, sizeof(unsigned char), 54, fileOut);

    // extract height, width and bitDepth of image from the header information
    int height = *(int*)&byte[18];
    int width = *(int*)&byte[22];
    int bitDepth = *(int*)&byte[28];
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
        
        // apply the threshold to each pixel in the chunk
        for(i = 0; i < bytesRead; i++) {
            buffer[i] = (buffer[i] > THRESHOLD)
                        ? WHITE
                        : BLACK;
        }
        // write the thresholded image data to the output file
        fwrite(buffer, sizeof(unsigned char), bytesRead, fileOut);
    }

    fclose(fileIn);
    fclose(fileOut);
    return 0;
}