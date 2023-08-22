#include <stdio.h>
#include <assert.h>

int main()
{
    // test 1: pixelate a small image
    assert(pixelate_image("test_input_small.bmp", "test_output_small.bmp") == 0);

    // test 2: pixelate a large image
    assert(pixelate_image("test_input_large.bmp", "test_output_large.bmp") == 0);

    // test 3: pixelate an image with odd dimensions
    assert(pixelate_image("test_input_odd.bmp", "test_output_odd.bmp") == 0);

    // test 4: pixelate an image with a non-standard header
    assert(pixelate_image("test_input_nonstandard.bmp", "test_output_nonstandard.bmp") == 0);

    // test 5: pixelate a grayscale image
    assert(pixelate_image("test_input_grayscale.bmp", "test_output_grayscale.bmp") == 0);

    printf("All tests passed successfully.\n");

    return 0;
}