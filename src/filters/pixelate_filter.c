#include <stdio.h>
#include <math.h>

#define MAX_COLOR 255
#define PIXEL_SIZE 10

int pixelate_filter(const char *input_file, const char *output_file)
{
    FILE *file_in = fopen(input_file, "rb");
    FILE *file_out = fopen(output_file, "wb+");
    unsigned char header_info[54];
    int i, j, r, g, b;

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
    for (i = 0; i < 54; i++)
    {
        header_info[i] = getc(file_in);
    }
    // write header info to output file
    fwrite(header_info, sizeof(unsigned char), 54, file_out);
    // extract attributes from image header
    int height = *(int *)&header_info[18];
    int width = *(int *)&header_info[22];
    int pixels_in_image = height * width;
    // check if image has a color table
    unsigned char buffer[pixels_in_image][3];
    // read & write image data in chunks until end of file is reached
    for (i = 0; i < height; i += PIXEL_SIZE)
    {
        for (j = 0; j < width; j += PIXEL_SIZE)
        {
            r = 0;
            g = 0;
            b = 0;
            // calculate average color of pixel block
            int count = 0;
            int x, y;
            for (x = i; x < i + PIXEL_SIZE; x++)
            {
                for (y = j; y < j + PIXEL_SIZE; y++)
                {
                    if (x < height && y < width)
                    {
                        r += buffer[x * width + y][0];
                        g += buffer[x * width + y][1];
                        b += buffer[x * width + y][2];
                        count++;
                    }
                }
            }
            r /= count;
            g /= count;
            b /= count;
            // set color of pixel block
            for (x = i; x < i + PIXEL_SIZE; x++)
            {
                for (y = j; y < j + PIXEL_SIZE; y++)
                {
                    if (x < height && y < width)
                    {
                        buffer[x * width + y][0] = r;
                        buffer[x * width + y][1] = g;
                        buffer[x * width + y][2] = b;
                    }
                }
            }
        }
    }
    // write image data to output file
    for (i = 0; i < pixels_in_image; i++)
    {
        putc(buffer[i][2], file_out); // blue
        putc(buffer[i][1], file_out); // green
        putc(buffer[i][0], file_out); // red
    }
    fclose(file_in);
    fclose(file_out);

    return 0;
}