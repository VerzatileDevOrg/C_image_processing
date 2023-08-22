#include <stdio.h>

int negative_filter(const char *input_file, const char *outputfile)
{
    FILE *file_in = fopen(input_file, "rb");
    FILE *file_out = fopen(outputfile, "wb+");

    if (file_in == NULL || file_out == NULL)
    {
        printf("File does not exist.\n");
        if (file_in != NULL)
            fclose(file_in);
        if (file_out != NULL)
            fclose(file_out);
        return 1;
    }
    unsigned char *image_data = NULL;
    unsigned char *new_image_data = NULL;
    unsigned char header_info[54];
    unsigned char color_table[1024];
    // read image header
    fread(header_info, sizeof(unsigned char), 54, file_in);
    int width = *(int *)&header_info[18];
    int height = *(int *)&header_info[22];
    int bit_depth = *(int *)&header_info[28];
    int image_data_size = width * height * bit_depth;
    // allocate memory for image data
    image_data = (unsigned char *)malloc(image_data_size * sizeof(unsigned char));
    new_image_data = (unsigned char *)malloc(image_data_size * sizeof(unsigned char));
    // read color table if present
    if (bit_depth <= 8)
    {
        fread(color_table, sizeof(unsigned char), 1024, file_in);
    }
    // read original image data
    fread(image_data, sizeof(unsigned char), image_data_size, file_in);
    // apply negative filter to each pixel
    unsigned char *p = image_data;
    unsigned char *q = new_image_data;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int index = (y * width + x) * (bit_depth / 8);

            for (int channel = 0; channel < bit_depth / 8; channel++)
            {
                new_image_data[index + channel] = 255 - image_data[index + channel];
            }
        }
    }
    // write image data to output file
    fwrite(header_info, sizeof(unsigned char), 54, file_out);
    if (bit_depth <= 8)
    {
        fwrite(color_table, sizeof(unsigned char), 1024, file_out);
    }
    fwrite(new_image_data, sizeof(unsigned char), image_data_size, file_out);
    // clean up memory
    fclose(file_in);
    fclose(file_out);
    free(image_data);
    free(new_image_data);

    return 0;
}