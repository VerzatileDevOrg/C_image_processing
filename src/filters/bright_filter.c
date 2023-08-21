#include <stdio.h>
#include <stdlib.h>

#define MAX_COLOR 255
#define BRIGHTNESS 25
#define CHUNK_SIZE 1024 // define size of the chunks to read and write
#define THRESHOLD 128   // define threshold value for the brightness condition

int bright_filter(const char *input_file, const char *output_file)
{
    FILE *file_in = fopen(input_file, "rb");
    FILE *file_out = fopen(output_file, "wb+");

    if (file_in == NULL || file_out == NULL)
    {
        printf("File does not exist.\n");
        if (file_in != NULL)
            fclose(file_in);
        if (file_out != NULL)
            fclose(file_out);
        return 1;
    }
    // read header info of image
    unsigned char header_info[54];
    unsigned char color_table[1024];
    // write header info to output file
    for (int i = 0; i < 54; i++)
    {
        header_info[i] = getc(file_in);
    }
    fwrite(header_info, sizeof(unsigned char), 54, file_out);
    // extract attributes from image header
    int height = *(int *)&header_info[18];
    int width = *(int *)&header_info[22];
    int bit_depth = *(int *)&header_info[28];
    int pixels_in_image = height * width;
    // check if image has a color table
    if (bit_depth <= 8)
    {
        fread(color_table, sizeof(unsigned char), 1024, file_in);
        fwrite(color_table, sizeof(unsigned char), 1024, file_out);
    }
    // allocate memory for image data
    unsigned char chunk_buffer[CHUNK_SIZE];
    // read & write image data in chunks until the end of file is reached
    while (!feof(file_in))
    {
        // read a chunk of image data from the input file
        size_t bytes_read = fread(chunk_buffer, sizeof(unsigned char), CHUNK_SIZE, file_in);
        // apply brightness factor to each pixel in the chunk
        for (int i = 0; i < bytes_read; i++)
        {
            chunk_buffer[i] = chunk_buffer[i] + BRIGHTNESS;
            chunk_buffer[i] = (chunk_buffer[i] > THRESHOLD) ? MAX_COLOR : chunk_buffer[i];
        }
        // write thresholded image data to output file
        fwrite(chunk_buffer, sizeof(unsigned char), bytes_read, file_out);
    }
    // write thresholded image data to output file
    fwrite(chunk_buffer, sizeof(unsigned char), pixels_in_image, file_out);
    // clean up memory
    fclose(file_in);
    fclose(file_out);

    return 0;
}