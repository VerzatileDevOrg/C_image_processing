#include "filters/black_and_white_filter.c"
#include "filters/negative_filter.c"

int main() {
    black_and_white_filter("assets/images/test_image.bmp", "assets/images/black_and_white_test_image.bmp");
    negative_filter("assets/images/test_image.bmp", "assets/images/negative_test_image.bmp");

    return 0;
}