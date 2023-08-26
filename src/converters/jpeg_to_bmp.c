#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>

int jpeg_to_bmp(const char *input_filename, const char *output_filename)
{
    FILE *input_file = fopen(input_filename, "rb");
    if (!input_file)
    {
        fprintf(stderr, "Error: could not open input file %s\n", input_filename);
        return 1;
    }
    
    // initialize JPEG decompressor object
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    
    // specify input file for JPEG decompressor
    jpeg_stdio_src(&cinfo, input_file);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);
    
    // get image dimensions
    int width = cinfo.output_width;
    int height = cinfo.output_height;
    
    // allocate memory for JPEG image data
    unsigned char *jpeg_data = (unsigned char *)malloc(width * height * cinfo.output_components);
    if (!jpeg_data)
    {
        fprintf(stderr, "Error: could not allocate memory for JPEG image data\n");
        fclose(input_file);
        jpeg_destroy_decompress(&cinfo);
        return 1;
    }
    
    // read JPEG image data
    while (cinfo.output_scanline < cinfo.output_height)
    {
        JSAMPROW row_pointer = &jpeg_data[(cinfo.output_height - cinfo.output_scanline - 1) * width * cinfo.output_components];
        jpeg_read_scanlines(&cinfo, &row_pointer, 1);
    }
    
    // finish decompression
    jpeg_finish_decompress(&cinfo);
    
    // close JPEG file
    fclose(input_file);
    jpeg_destroy_decompress(&cinfo);
    
    // open output BMP file for writing
    FILE *output_file = fopen(output_filename, "wb");
    if (!output_file)
    {
        fprintf(stderr, "Error: could not open output file %s\n", output_filename);
        free(jpeg_data);
        return 1;
    }
    
    // write BMP header
    unsigned char bmp_header[54] = {
        'B', 'M',                     // signature
        0, 0, 0, 0,                   // file size (to be filled later)
        0, 0, 0, 0,                   // reserved
        54, 0, 0, 0,                  // offset to pixel data
        40, 0, 0, 0,                  // header size
        0, 0, 0, 0,                   // image width (to be filled later)
        0, 0, 0, 0,                   // image height (to be filled later)
        1, 0,                         // number of color planes
        24, 0,                        // bits per pixel (3 bytes)
        0, 0, 0, 0,                   // compression method
        0, 0, 0, 0,                   // image size (can be 0)
        0, 0, 0, 0,                   // horizontal resolution (can be 0)
        0, 0, 0, 0,                   // vertical resolution (can be 0)
        0, 0, 0, 0,                   // number of colors in palette (0 for 24-bit)
        0, 0, 0, 0                    // number of important colors (can be 0)
    };
    
    int bmp_file_size = sizeof(bmp_header) + width * height * 3;
    bmp_header[2] = (unsigned char)(bmp_file_size);
    bmp_header[3] = (unsigned char)(bmp_file_size >> 8);
    bmp_header[4] = (unsigned char)(bmp_file_size >> 16);
    bmp_header[5] = (unsigned char)(bmp_file_size >> 24);
    bmp_header[18] = (unsigned char)(width);
    bmp_header[19] = (unsigned char)(width >> 8);
    bmp_header[20] = (unsigned char)(width >> 16);
    bmp_header[21] = (unsigned char)(width >> 24);
    bmp_header[22] = (unsigned char)(height);
    bmp_header[23] = (unsigned char)(height >> 8);
    bmp_header[24] = (unsigned char)(height >> 16);
    bmp_header[25] = (unsigned char)(height >> 24);
    
    fwrite(bmp_header, sizeof(unsigned char), sizeof(bmp_header), output_file);
    
    // write BMP image data
    for (int y = height - 1; y >= 0; y--)
    {
        for (int x = 0; x < width; x++)
        {
            int jpeg_index = (height - y - 1) * width * cinfo.output_components + x * cinfo.output_components;
            fwrite(&jpeg_data[jpeg_index + 2], sizeof(unsigned char), 1, output_file); // Blue
            fwrite(&jpeg_data[jpeg_index + 1], sizeof(unsigned char), 1, output_file); // Green
            fwrite(&jpeg_data[jpeg_index + 0], sizeof(unsigned char), 1, output_file); // Red
        }
        
        // write padding if necessary (multiple of 4 bytes)
        for (int p = 0; p < (4 - (width * 3) % 4) % 4; p++)
        {
            fputc(0, output_file);
        }
    }
    
    fclose(output_file);
    printf("Converted %s to %s\n", input_filename, output_filename);
    
    return 0;
}