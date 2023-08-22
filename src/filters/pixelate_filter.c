#include <stdio.h>
#include <stdlib.h>

#define MAX_COLOR 255
#define PIXEL_SIZE 20

int pixelate_filter(const char *input_file, const char *output_file)
{
    FILE *file_in = fopen(input_file, "rb");
    FILE *file_out = fopen(output_file, "wb");

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
    // write header info to output file
    fread(header_info, sizeof(unsigned char), 54, file_in);
    // extract attributes from image header
    int height = *(int *)&header_info[18];
    int width = *(int *)&header_info[22];
    int bits_per_pixel = *(int *)&header_info[28];
    int pixels_in_image = height * width;
    // write header info to output file
    fwrite(header_info, sizeof(unsigned char), 54, file_out);
    // check if image has a color table
    unsigned char *pixel_data = (unsigned char *)malloc(pixels_in_image * (bits_per_pixel / 8));
    fread(pixel_data, sizeof(unsigned char), pixels_in_image * (bits_per_pixel / 8), file_in);
    // read & write image data in chunks until end of file reached
    for (int i = 0; i < height; i += PIXEL_SIZE)
    {
        for (int j = 0; j < width; j += PIXEL_SIZE)
        {
            int r = 0, g = 0, b = 0;
            int count = 0;
            for (int x = i; x < i + PIXEL_SIZE && x < height; x++)
            {
                for (int y = j; y < j + PIXEL_SIZE && y < width; y++)
                {
                    int index = (x * width + y) * (bits_per_pixel / 8);
                    b += pixel_data[index];
                    g += pixel_data[index + 1];
                    r += pixel_data[index + 2];
                    count++;
                }
            }
            r /= count;
            g /= count;
            b /= count;

            for (int x = i; x < i + PIXEL_SIZE && x < height; x++)
            {
                for (int y = j; y < j + PIXEL_SIZE && y < width; y++)
                {
                    int index = (x * width + y) * (bits_per_pixel / 8);
                    pixel_data[index] = b;
                    pixel_data[index + 1] = g;
                    pixel_data[index + 2] = r;
                }
            }
        }
    }
    // write image data to output file
    fwrite(pixel_data, sizeof(unsigned char), pixels_in_image * (bits_per_pixel / 8), file_out);
    // clean up memory
    free(pixel_data);
    fclose(file_in);
    fclose(file_out);

    return 0;
}