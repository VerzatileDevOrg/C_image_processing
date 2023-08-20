#include "filters/black_and_white_filter.c"
#include "filters/negative_filter.c"
#include <gtk/gtk.h>

int main() {
    
    const char* inputFileName = "assets/images/test_image.bmp";
    const char* outputFileName = "assets/images/output.bmp";
    int result = black_and_white_filter(inputFileName, outputFileName);

    switch (result) {
    case 0:
        printf("Black and white filtering completed successfully.\n");
        break;
    case 1:
        printf("Error: Input file \"%s\" not found.\n", inputFileName);
        break;
    case 2:
        printf("Error: Unable to create output file \"%s\".\n", outputFileName);
        break;
    default:
        printf("Error: An unknown error occurred during filtering.\n");
    }

    return 0;
}