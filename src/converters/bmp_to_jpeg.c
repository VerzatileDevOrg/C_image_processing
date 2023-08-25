#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>

int bmp_to_jpeg(const char *input_filename, const char *output_filename)
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
    // allocate memory for JPEG image data
    unsigned char *jpeg_data = NULL;
    unsigned long jpeg_size = 0;
    // create JPEG compressor object
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    // open output file for writing
    FILE *output_file = fopen(output_filename, "wb");
    if (!output_file)
    {
        fprintf(stderr, "Error: could not open output file %s\n", output_filename);
        free(bmp_data);
        jpeg_destroy_compress(&cinfo);
        return 1;
    }
    // set JPEG compressor parameters
    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, 75, TRUE);
    // set output file for JPEG compressor
    jpeg_stdio_dest(&cinfo, output_file);
    // start JPEG compressor
    jpeg_start_compress(&cinfo, TRUE);
    
    // write JPEG image data
    while (cinfo.next_scanline < cinfo.image_height)
    {
        // Get a pointer to the current row of BMP data
        unsigned char *bmp_row = &bmp_data[(cinfo.image_height - cinfo.next_scanline - 1) * (width * 3 + padding)];
        
        // Allocate an array to hold a row of RGB data in the correct order (RGB)
        unsigned char *rgb_row = (unsigned char *)malloc(width * 3);

        // Extract RGB data from BGR order and store it in rgb_row
        for (int i = 0; i < width; i++)
        {
            rgb_row[i * 3 + 0] = bmp_row[i * 3 + 2]; // Blue
            rgb_row[i * 3 + 1] = bmp_row[i * 3 + 1]; // Green
            rgb_row[i * 3 + 2] = bmp_row[i * 3 + 0]; // Red
        }
        
        // Write the corrected RGB row to the JPEG compressor
        JSAMPROW row_pointer = rgb_row;
        jpeg_write_scanlines(&cinfo, &row_pointer, 1);
        
        // Free the allocated memory for the RGB row
        free(rgb_row);
    }
    jpeg_finish_compress(&cinfo);
    jpeg_size = ftell(output_file);
    fclose(output_file);
    jpeg_destroy_compress(&cinfo);
    free(bmp_data);
    printf("Converted %s to %s (%lu bytes)\n", input_filename, output_filename, jpeg_size);

    return 0;
}