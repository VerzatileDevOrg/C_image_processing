#include <stdio.h>
#include <stdlib.h>
#include <png.h>

int png_to_bmp(const char *input_filename, const char *output_filename)
{
    // open PNG file for reading
    FILE *input_file = fopen(input_filename, "rb");
    if (!input_file)
    {
        fprintf(stderr, "Error: could not open input file %s\n", input_filename);
        return 1;
    }
    // create PNG read struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
        fprintf(stderr, "Error: could not create PNG read struct\n");
        fclose(input_file);
        return 1;
    }
    // create PNG info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        fprintf(stderr, "Error: could not create PNG info struct\n");
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(input_file);
        return 1;
    }
    // set error handling
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        fprintf(stderr, "Error: PNG error\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(input_file);
        return 1;
    }
    // initialize PNG I/O
    png_init_io(png_ptr, input_file);
    // read PNG header info
    png_read_info(png_ptr, info_ptr);
    // get PNG image attributes
    int width = png_get_image_width(png_ptr, info_ptr);
    int height = png_get_image_height(png_ptr, info_ptr);
    int color_type = png_get_color_type(png_ptr, info_ptr);
    int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    // Make sure it's a valid format for conversion
    if (color_type != PNG_COLOR_TYPE_RGB || bit_depth != 8)
    {
        fprintf(stderr, "Error: unsupported PNG format\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(input_file);
        return 1;
    }
    // allocate memory for PNG image data
    png_bytep *row_pointers = (png_bytep *)malloc(height * sizeof(png_bytep));
    if (!row_pointers)
    {
        fprintf(stderr, "Error: could not allocate memory for PNG image data\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(input_file);
        return 1;
    }
    for (int y = 0; y < height; y++)
    {
        row_pointers[y] = (png_byte *)malloc(png_get_rowbytes(png_ptr, info_ptr));
        if (!row_pointers[y])
        {
            fprintf(stderr, "Error: could not allocate memory for PNG image data\n");
            for (int i = 0; i < y; i++)
                free(row_pointers[i]);
            free(row_pointers);
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            fclose(input_file);
            return 1;
        }
    }
    // read PNG image data
    png_read_image(png_ptr, row_pointers);
    // close PNG file
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(input_file);
    // open BMP file for writing
    FILE *output_file = fopen(output_filename, "wb");
    if (!output_file)
    {
        fprintf(stderr, "Error: could not open output file %s\n", output_filename);
        for (int y = 0; y < height; y++)
            free(row_pointers[y]);
        free(row_pointers);
        return 1;
    }
    // write BMP header
    unsigned char header[54] = {
        'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0, 40, 0,
        0, 0, (unsigned char)(width), (unsigned char)(width >> 8), (unsigned char)(width >> 16), (unsigned char)(width >> 24),
        (unsigned char)(height), (unsigned char)(height >> 8), (unsigned char)(height >> 16), (unsigned char)(height >> 24),
        1, 0, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    fwrite(header, sizeof(unsigned char), 54, output_file);
    // write BMP image data
    for (int y = height - 1; y >= 0; y--)
    {
        for (int x = 0; x < width; x++)
        {
            png_bytep pixel = &(row_pointers[y][x * 3]);
            fwrite(&pixel[2], sizeof(png_byte), 1, output_file);
            fwrite(&pixel[1], sizeof(png_byte), 1, output_file);
            fwrite(&pixel[0], sizeof(png_byte), 1, output_file);
        }
        // Add padding
        for (int p = 0; p < (4 - (width * 3) % 4) % 4; p++)
            fputc(0, output_file);
    }
    // close BMP file
    fclose(output_file);
    // free memory
    for (int y = 0; y < height; y++)
        free(row_pointers[y]);
    free(row_pointers);
    printf("Converted %s to %s\n", input_filename, output_filename);
    return 0;
}