#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>

int bmp_to_jpeg(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s input.bmp output.jpg\n", argv[0]);
        return 1;
    }

    char *input_filename = argv[1];
    char *output_filename = argv[2];

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
        JSAMPROW row_pointer = &bmp_data[(cinfo.image_height - cinfo.next_scanline - 1) * (width * 3 + padding)];
        jpeg_write_scanlines(&cinfo, &row_pointer, 1);
    }
    // finish JPEG compressor
    jpeg_finish_compress(&cinfo);
    // get size of JPEG image data
    jpeg_size = ftell(output_file);
    // close output file
    fclose(output_file);
    // destroy JPEG compressor object
    jpeg_destroy_compress(&cinfo);
    // free BMP image data
    free(bmp_data);
    printf("Converted %s to %s (%lu bytes)\n", input_filename, output_filename, jpeg_size);

    return 0;
}