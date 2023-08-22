#include <stdio.h>

#define MAX_COLOR 255
#define DARK_FACTOR 0.4

int dark_filter(const char *input_file, const char *output_file)
{
    FILE *file_in = fopen(input_file, "rb");
    FILE *file_out = fopen(output_file, "wb+");
    unsigned char header_info[54];
    int i, r, g, b;

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
    // allocate memory to image data
    unsigned char buffer[pixels_in_image][3];
    // read & write image data in chunks until end of file is reached
    for (i = 0; i < pixels_in_image; i++)
    {
        r = 0;
        g = 0;
        b = 0;
        buffer[i][2] = getc(file_in); // blue
        buffer[i][1] = getc(file_in); // green
        buffer[i][0] = getc(file_in); // red
        // apply darkness effect to each channel
        r = buffer[i][0] * DARK_FACTOR;
        g = buffer[i][1] * DARK_FACTOR;
        b = buffer[i][2] * DARK_FACTOR;
        // Write darkened image data to output file
        putc(b, file_out);
        putc(g, file_out);
        putc(r, file_out);
    }
    // clean up memory
    fclose(file_out);
    fclose(file_in);

    return 0;
}