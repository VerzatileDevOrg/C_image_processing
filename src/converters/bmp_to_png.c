#include <stdio.h>
#include <stdlib.h>
#include <png.h>

int bmp_to_png(const char *input_filename, const char *output_filename)
{
    // open BMP file for reading
    FILE *input_file = fopen(input_filename, "rb");
    if (!input_file)
    {
        fprintf(stderr, "Error: could not open input file %s\n", input_filename);
        return 1;
    }
    // read BMP header
    unsigned char header[54];
    if (fread(header, sizeof(unsigned char), 54, input_file) != 54)
    {
        fprintf(stderr, "Error: invalid BMP file %s\n", input_filename);
        fclose(input_file);
        return 1;
    }
    // check if BMP file is valid
    if (header[0] != 'B' || header[1] != 'M')
    {
        fprintf(stderr, "Error: invalid BMP file %s\n", input_filename);
        fclose(input_file);
        return 1;
    }
    // get image width and height from BMP header
    int width = *(int *)&header[18];
    int height = *(int *)&header[22];
    // calculate padding for BMP file
    int padding = 0;
    while ((width * 3 + padding) % 4 != 0)
    {
        padding++;
    }
    // allocate memory for BMP image data
    unsigned char *bmp_data = (unsigned char *)malloc(width * height * 3 + height * padding);
    if (!bmp_data)
    {
        fprintf(stderr, "Error: could not allocate memory for BMP image data\n");
        fclose(input_file);
        return 1;
    }
    // read BMP image data
    if (fread(bmp_data, sizeof(unsigned char), width * height * 3 + height * padding, input_file) != width * height * 3 + height * padding)
    {
        fprintf(stderr, "Error: invalid BMP file %s\n", input_filename);
        free(bmp_data);
        fclose(input_file);
        return 1;
    }
    // close BMP file
    fclose(input_file);
    // open PNG file for writing
    FILE *output_file = fopen(output_filename, "wb");
    if (!output_file)
    {
        fprintf(stderr, "Error: could not open output file %s\n", output_filename);
        free(bmp_data);
        return 1;
    }
    // create PNG write struct
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
        fprintf(stderr, "Error: could not create PNG write struct\n");
        free(bmp_data);
        fclose(output_file);
        return 1;
    }
    // create PNG info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        fprintf(stderr, "Error: could not create PNG info struct\n");
        free(bmp_data);
        png_destroy_write_struct(&png_ptr, NULL);
        fclose(output_file);
        return 1;
    }
    // set error handling
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        fprintf(stderr, "Error: PNG error\n");
        free(bmp_data);
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(output_file);
        return 1;
    }
    // set PNG output file
    png_init_io(png_ptr, output_file);
    // set PNG header info
    png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    // write PNG header info
    png_write_info(png_ptr, info_ptr);
    // write PNG image data
    for (int y = 0; y < height; y++) {
    png_bytep row_pointer = &bmp_data[(height - y - 1) * (width * 3 + padding)];

    // Convert BGR to RGB
    for (int x = 0; x < width; x++) {
        png_bytep pixel = &row_pointer[x * 3];
        png_byte temp = pixel[0];
        pixel[0] = pixel[2];
        pixel[2] = temp;
    }

    png_write_row(png_ptr, row_pointer);
    }
    // finish writing PNG file
    png_write_end(png_ptr, NULL);
    // free PNG write struct and info struct
    png_destroy_write_struct(&png_ptr, &info_ptr);
    // free BMP image data
    free(bmp_data);
    // close PNG file
    fclose(output_file);
    printf("Converted %s to %s\n", input_filename, output_filename);

    return 0;
}