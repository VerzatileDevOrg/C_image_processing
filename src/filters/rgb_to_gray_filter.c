#include <stdio.h>

int rgb_to_gray_filter(const char *input_file, const char *output_file)
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
    // read and write header info
    unsigned char header_info[54];
    fread(header_info, sizeof(unsigned char), 54, file_in);
    fwrite(header_info, sizeof(unsigned char), 54, file_out);
    // extract image dimensions from header info
    int height = *(int *)&header_info[18];
    int width = *(int *)&header_info[22];
    int pixels_in_image = height * width;
    // allocate memory to image data
    unsigned char(*buffer)[3] = malloc(pixels_in_image * sizeof(*buffer));
    if (buffer == NULL)
    {
        printf("Memory allocation failed.\n");
        fclose(file_in);
        fclose(file_out);
        return 1;
    }
    for (int i = 0; i < pixels_in_image; i++)
    {
        // read RGB color components of current pixel from input file
        buffer[i][0] = getc(file_in); // red
        buffer[i][1] = getc(file_in); // green
        buffer[i][2] = getc(file_in); // blue
        // calculate grayscale value using weighted average of red and blue components
        unsigned char grayscale_value = (buffer[i][0] * 0.3) + (buffer[i][2] * 0.11);
        // write the calculated grayscale value to the output file for each RGB channel
        putc(grayscale_value, file_out); // red
        putc(grayscale_value, file_out); // green
        putc(grayscale_value, file_out); // blue
    }
    // clean up memory
    fclose(file_in);
    fclose(file_out);

    return 0;
}