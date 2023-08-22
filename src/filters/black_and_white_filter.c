#include <stdio.h>

#define THRESHOLD 128 // define value of threshold for black and white
#define WHITE 255
#define BLACK 0
#define CHUNK_SIZE 1024 // define size of chunks to read and write

int black_and_white_filter(const char *input_file, const char *output_file)
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
    unsigned char header_info[54];
    unsigned char color_table[1024];
    // read header info of image
    for (int i = 0; i < 54; i++)
    {
        header_info[i] = getc(file_in);
    }
    // write header info to output file
    fwrite(header_info, sizeof(unsigned char), 54, file_out);
    // extract attributes from image header
    int height = *(int *)&header_info[18];
    int width = *(int *)&header_info[22];
    int bit_depth = *(int *)&header_info[28];
    // check if the image has a color table
    if (bit_depth <= 8)
    {
        fread(color_table, sizeof(unsigned char), 1024, file_in);
        fwrite(color_table, sizeof(unsigned char), 1024, file_out);
    }
    // allocate memory for image data
    unsigned char chunk_buffer[CHUNK_SIZE];
    // read & write the image data in chunks until the end of file is reached
    while (!feof(file_in))
    {
        // read a chunk of image data from input file
        size_t bytes_read = fread(chunk_buffer, sizeof(unsigned char), CHUNK_SIZE, file_in);
        // apply threshold to each pixel in the chunk
        for (int i = 0; i < bytes_read; i++)
        {
            chunk_buffer[i] = (chunk_buffer[i] > THRESHOLD) ? WHITE : BLACK;
        }
        // write the thresholded image data to the output file
        fwrite(chunk_buffer, sizeof(unsigned char), bytes_read, file_out);
    }
    // clean up memory
    fclose(file_in);
    fclose(file_out);

    return 0;
}