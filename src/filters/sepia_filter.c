#include <stdio.h>

#define MAX_VALUE 255

int sepia_filter(const char *input_file, const char *output_file)
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
    // check if image has a color table
    unsigned char(*buffer)[3] = malloc(pixels_in_image * sizeof(*buffer));
    if (buffer == NULL)
        // read & write image data in chunks until end of file reached
        for (i = 0; i < pixels_in_image; i++)
        {
            r = 0;
            g = 0;
            b = 0;
            buffer[i][2] = getc(file_in); // blue
            buffer[i][1] = getc(file_in); // green
            buffer[i][0] = getc(file_in); // red
            // conversion formula of rgb to sepia
            r = (buffer[i][0] * 0.393) + (buffer[i][1] * 0.769) + (buffer[i][2] * 0.189);
            g = (buffer[i][0] * 0.349) + (buffer[i][1] * 0.686) + (buffer[i][2] * 0.168);
            b = (buffer[i][0] * 0.272) + (buffer[i][1] * 0.534) + (buffer[i][2] * 0.131);
            // check if value is greater than 255
            if (r > MAX_VALUE)
            {
                r = MAX_VALUE;
            }
            if (g > MAX_VALUE)
            {
                g = MAX_VALUE;
            }
            if (b > MAX_VALUE)
            {
                b = MAX_VALUE;
            }
            // write sepia image data to output file
            putc(b, file_out);
            putc(g, file_out);
            putc(r, file_out);
        }
    fclose(file_out);
    fclose(file_in);

    return 0;
}