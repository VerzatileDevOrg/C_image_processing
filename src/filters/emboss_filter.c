#include <stdio.h>

#define MAX_COLOR 255

int emboss_filter(const char *input_file, const char *output_file)
{
    FILE *file_in = fopen(input_file, "rb");
    FILE *file_out = fopen(output_file, "wb+");
    unsigned char header_info[54];
    int i, j, r, g, b, gray, prev_gray, diff;

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
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            // read pixel color
            b = getc(file_in);
            g = getc(file_in);
            r = getc(file_in);
            // convert to grayscale
            gray = 0.299 * r + 0.587 * g + 0.114 * b;
            if (i > 0 && j > 0)
            {
                // calculate difference between current and previous pixel grayscale values
                diff = gray - prev_gray;
                // calculate new pixel color
                r = g = b = MAX_COLOR / 2 + diff;
                // clamp color values to [0, MAX_COLOR]
                r = fmax(0, fmin(r, MAX_COLOR));
                g = fmax(0, fmin(g, MAX_COLOR));
                b = fmax(0, fmin(b, MAX_COLOR));
            }
            // set current grayscale value as previous grayscale value for next iteration
            prev_gray = gray;
            // set color of current pixel
            buffer[i * width + j][0] = r;
            buffer[i * width + j][1] = g;
            buffer[i * width + j][2] = b;
        }
    }
    // write image data to output file
    for (i = 0; i < pixels_in_image; i++)
    {
        putc(buffer[i][2], file_out); // blue
        putc(buffer[i][1], file_out); // green
        putc(buffer[i][0], file_out); // red
    }
    // clean up memory
    fclose(file_in);
    fclose(file_out);

    return 0;
}