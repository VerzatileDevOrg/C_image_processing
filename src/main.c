#include "gtk_gui.c"
//#include "converters/bmp_to_jpeg.c" // Include this line to test the bmp_to_jpeg function
//#include "converters/bmp_to_png.c"

int main(int argc, char **argv)
{
    //Include the following line to test the bmp_to_jpeg function
    //bmp_to_jpeg("assets/images/test_image.bmp", "assets/images/testjpg.jpg");
    //bmp_to_png("assets/images/test_image_negative.bmp", "assets/images/testpng.png");

    GtkApplication *app;
    int status;

    // create new GTK application
    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    // connect activate signal to activate function
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    // run application
    status = g_application_run(G_APPLICATION(app), argc, argv);
    // release application object
    g_object_unref(app);
    // wait for user input before exiting
    getchar();

    return status;
}