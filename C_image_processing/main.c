#include <gtk/gtk.h>

#include "filters/black_and_white_filter.c"
#include "filters/bright_filter.c"
#include "filters/dark_filter.c"
#include "filters/negative_filter.c"
#include "filters/rgb_to_gray_filter.c"
#include "filters/sepia_filter.c"

// callback functions for all filters
static void black_and_white_filter_apply(GtkWidget *widget, gpointer data)
{
    black_and_white_filter("assets/images/test_image.bmp", "assets/images/test_image_black_and_white.bmp");
    g_print("Black And White Filter Has Been Applied\n");
}

static void bright_filter_apply(GtkWidget *widget, gpointer data)
{
    bright_filter("assets/images/test_image.bmp", "assets/images/test_image_bright.bmp");
    g_print("Bright Filter Has Been Applied\n");
}

static void dark_filter_apply(GtkWidget *widget, gpointer data)
{
    dark_filter("assets/images/test_image.bmp", "assets/images/test_image_dark.bmp");
    g_print("Dark Filter Has Been Applied\n");
}

static void negative_filter_apply(GtkWidget *widget, gpointer data)
{
    negative_filter("assets/images/test_image.bmp", "assets/images/test_image_negative.bmp");
    g_print("Negative Filter Has Been Applied\n");
}

static void rgb_to_gray_filter_apply(GtkWidget *widget, gpointer data)
{
    rgb_to_gray_filter("assets/images/test_image.bmp", "assets/images/test_image_rgb_to_gray.bmp");
    g_print("RGB To Gray Filter Has Been Applied\n");
}

static void sepia_filter_apply(GtkWidget *widget, gpointer data)
{
    sepia_filter("assets/images/test_image.bmp", "assets/images/test_image_sepia.bmp");
    g_print("Sepia Filter Has Been Applied\n");
}

static void activate(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *image;
    GtkWidget *image_box;
    GtkWidget *button0;
    GtkWidget *button1;
    GtkWidget *button2;
    GtkWidget *button3;
    GtkWidget *button4;
    GtkWidget *button5;
    GtkWidget *box;
    GtkWidget *button_box;
    const char *css = "@font-face {"
                      "    font-family: BigNoodleTitling;"
                      "    src: url(\"assets/fonts/big_noodle_titling.ttf\");"
                      "}"
                      "headerbar.titlebar {"
                      "    font-family: BigNoodleTitling;"
                      "    font-size: 36px;"
                      "    color: black;"
                      "    background-color: #4CAF50;"
                      "    border: none;"
                      "    padding: 10px 20px;"
                      "    text-align: center;"
                      "    text-decoration: none;"
                      "    display: inline-block;"
                      "}"
                      "button {"
                      "    background-color: #4CAF50;"
                      "    color: black;"
                      "    2px solid #008CBA;"
                      "    padding: 10px 20px;"
                      "    text-align: center;"
                      "    text-decoration: none;"
                      "    display: inline-block;"
                      "    font-size: 22px;"
                      "    margin: 8px 5px;"
                      "    cursor: pointer;"
                      "    font-family: BigNoodleTitling;"
                      "}";

    // create new window
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "C Image Processing");
    gtk_window_set_default_size(GTK_WINDOW(window), 1000, 1000);

    // create new vertical box container
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_END);

    // include box container in window
    gtk_window_set_child(GTK_WINDOW(window), box);

    // create new horizontal box container for image
    image_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_halign(image_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(image_box, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_bottom(image_box, 40);

    // append image box to vertical box
    gtk_box_append(GTK_BOX(box), image_box);

    // create image widget
    image = gtk_image_new_from_file("assets/images/test_image.bmp");
    gtk_image_set_pixel_size(GTK_IMAGE(image), 1000);

    // append image widget to image box
    gtk_box_append(GTK_BOX(image_box), image);

    // create new horizontal box container for buttons
    button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_halign(button_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button_box, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_bottom(button_box, 40);

    // append button box to vertical box
    gtk_box_append(GTK_BOX(box), button_box);

    // create filter buttons
    button0 = gtk_button_new_with_label("Black And White Filter");
    button1 = gtk_button_new_with_label("Bright Filter");
    button2 = gtk_button_new_with_label("Dark Filter");
    button3 = gtk_button_new_with_label("Negative Filter");
    button4 = gtk_button_new_with_label("RGB To Gray Filter");
    button5 = gtk_button_new_with_label("Sepia Filter");
    // connect buttons to callback functions
    g_signal_connect(button0, "clicked", G_CALLBACK(black_and_white_filter_apply), NULL);
    g_signal_connect(button1, "clicked", G_CALLBACK(bright_filter_apply), NULL);
    g_signal_connect(button2, "clicked", G_CALLBACK(dark_filter_apply), NULL);
    g_signal_connect(button3, "clicked", G_CALLBACK(negative_filter_apply), NULL);
    g_signal_connect(button4, "clicked", G_CALLBACK(rgb_to_gray_filter_apply), NULL);
    g_signal_connect(button5, "clicked", G_CALLBACK(sepia_filter_apply), NULL);
    // append buttons to button box
    gtk_box_append(GTK_BOX(button_box), button0);
    gtk_box_append(GTK_BOX(button_box), button1);
    gtk_box_append(GTK_BOX(button_box), button2);
    gtk_box_append(GTK_BOX(button_box), button3);
    gtk_box_append(GTK_BOX(button_box), button4);
    gtk_box_append(GTK_BOX(button_box), button5);

    // create new CSS provider object and load CSS
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(provider, css);
    // add CSS provider to default display
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // show window
    gtk_widget_set_visible(window, TRUE);
}

int main(int argc, char **argv)
{
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