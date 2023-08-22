#include <gtk/gtk.h>

#include "filters/black_and_white_filter.c"
#include "filters/bright_filter.c"
#include "filters/dark_filter.c"
#include "filters/emboss_filter.c"
#include "filters/negative_filter.c"
#include "filters/pixelate_filter.c"
#include "filters/rgb_to_gray_filter.c"
#include "filters/sepia_filter.c"

// callback functions for all filters
static void black_and_white_filter_apply(GtkWidget *widget, gpointer data)
{
    GtkWidget *image = data;
    black_and_white_filter("assets/images/test_image.bmp", "assets/images/test_image_black_and_white.bmp");
    gtk_image_set_from_file(GTK_IMAGE(image), "assets/images/test_image_black_and_white.bmp");
    g_print("Black And White Filter Has Been Applied\n");
}

static void bright_filter_apply(GtkWidget *widget, gpointer data)
{
    GtkWidget *image = data;
    bright_filter("assets/images/test_image.bmp", "assets/images/test_image_bright.bmp");
    gtk_image_set_from_file(GTK_IMAGE(image), "assets/images/test_image_bright.bmp");
    g_print("Bright Filter Has Been Applied\n");
}

static void dark_filter_apply(GtkWidget *widget, gpointer data)
{
    GtkWidget *image = data;
    dark_filter("assets/images/test_image.bmp", "assets/images/test_image_dark.bmp");
    gtk_image_set_from_file(GTK_IMAGE(image), "assets/images/test_image_dark.bmp");
    g_print("Dark Filter Has Been Applied\n");
}

static void emboss_filter_apply(GtkWidget *widget, gpointer data)
{
    GtkWidget *image = data;
    emboss_filter("assets/images/test_image.bmp", "assets/images/test_image_emboss.bmp");
    gtk_image_set_from_file(GTK_IMAGE(image), "assets/images/test_image_emboss.bmp");
    g_print("Emboss Filter Has Been Applied\n");
}

static void negative_filter_apply(GtkWidget *widget, gpointer data)
{
    GtkWidget *image = data;
    negative_filter("assets/images/test_image.bmp", "assets/images/test_image_negative.bmp");
    gtk_image_set_from_file(GTK_IMAGE(image), "assets/images/test_image_negative.bmp");
    g_print("Negative Filter Has Been Applied\n");
}

static void pixelate_filter_apply(GtkWidget *widget, gpointer data)
{
    GtkWidget *image = data;
    pixelate_filter("assets/images/test_image.bmp", "assets/images/test_image_pixelate.bmp");
    gtk_image_set_from_file(GTK_IMAGE(image), "assets/images/test_image_pixelate.bmp");
    g_print("Pixelate Filter Has Been Applied\n");
}

static void rgb_to_gray_filter_apply(GtkWidget *widget, gpointer data)
{
    GtkWidget *image = data;
    rgb_to_gray_filter("assets/images/test_image.bmp", "assets/images/test_image_rgb_to_gray.bmp");
    gtk_image_set_from_file(GTK_IMAGE(image), "assets/images/test_image_rgb_to_gray.bmp");
    g_print("RGB To Gray Filter Has Been Applied\n");
}

static void sepia_filter_apply(GtkWidget *widget, gpointer data)
{
    GtkWidget *image = data;
    sepia_filter("assets/images/test_image.bmp", "assets/images/test_image_sepia.bmp");
    gtk_image_set_from_file(GTK_IMAGE(image), "assets/images/test_image_sepia.bmp");
    g_print("Sepia Filter Has Been Applied\n");
}

static void activate(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *image;
    GtkWidget *user_image_box;
    GtkWidget *filtered_image_box;
    GtkWidget *button_box0;
    GtkWidget *button_box1;
    GtkWidget *button0;
    GtkWidget *button1;
    GtkWidget *button2;
    GtkWidget *button3;
    GtkWidget *button4;
    GtkWidget *button5;
    GtkWidget *button6;
    GtkWidget *button7;
    const char *css = "window {"
                      "    background-color: #FFFFE4;"
                      "}"
                      "headerbar.titlebar {"
                      "    font-family: BigNoodleTitling;"
                      "    font-size: 36px;"
                      "    color: black;"
                      "    background-color: #4CAF50;"
                      "    border: none;"
                      "    padding: 10px 20px;"
                      "    text-decoration: none;"
                      "}"
                      "button {"
                      "    background-color: #4CAF50;"
                      "    color: black;"
                      "    2px solid #008CBA;"
                      "    padding: 10px 20px;"
                      "    text-decoration: none;"
                      "    font-size: 22px;"
                      "    margin: 8px 5px;"
                      "    font-family: BigNoodleTitling;"
                      "}";

    // create new window
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "C Image Processing");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    // create new vertical box container
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_END);
    // include box container in window
    gtk_window_set_child(GTK_WINDOW(window), box);

    // create new horizontal box container for user image
    user_image_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_halign(user_image_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(user_image_box, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_bottom(user_image_box, 40);
    // append user image box to vertical box
    gtk_box_append(GTK_BOX(box), user_image_box);
    // create image widget
    image = gtk_image_new_from_file("assets/images/test_image.bmp");
    gtk_image_set_pixel_size(GTK_IMAGE(image), 450);
    // append user image widget to user image box
    gtk_box_append(GTK_BOX(user_image_box), image);

    // create new horizontal box container for filtered image
    filtered_image_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_halign(filtered_image_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(filtered_image_box, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_bottom(filtered_image_box, 40);
    // append filtered image box to vertical box
    gtk_box_append(GTK_BOX(box), filtered_image_box);
    // create image widget
    image = gtk_image_new_from_file("assets/images/test_image.bmp");
    gtk_image_set_pixel_size(GTK_IMAGE(image), 450);
    // append user image widget to filtered image box
    gtk_box_append(GTK_BOX(filtered_image_box), image);

    // create new horizontal box container for buttons
    button_box0 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_halign(button_box0, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button_box0, GTK_ALIGN_CENTER);
    // append button box to vertical box
    gtk_box_append(GTK_BOX(box), button_box0);

    // create new horizontal box container for buttons
    button_box1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_halign(button_box1, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button_box1, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_bottom(button_box1, 20);
    // append button box to vertical box
    gtk_box_append(GTK_BOX(box), button_box1);

    // create filter buttons
    button0 = gtk_button_new_with_label("Black And White Filter");
    button1 = gtk_button_new_with_label("Bright Filter");
    button2 = gtk_button_new_with_label("Dark Filter");
    button3 = gtk_button_new_with_label("Emboss Filter");
    button4 = gtk_button_new_with_label("Negative Filter");
    button5 = gtk_button_new_with_label("Pixelate Filter");
    button6 = gtk_button_new_with_label("RGB To Gray Filter");
    button7 = gtk_button_new_with_label("Sepia Filter");
    // connect buttons to callback functions
    g_signal_connect(button0, "clicked", G_CALLBACK(black_and_white_filter_apply), image);
    g_signal_connect(button1, "clicked", G_CALLBACK(bright_filter_apply), image);
    g_signal_connect(button2, "clicked", G_CALLBACK(dark_filter_apply), image);
    g_signal_connect(button3, "clicked", G_CALLBACK(emboss_filter_apply), image);
    g_signal_connect(button4, "clicked", G_CALLBACK(negative_filter_apply), image);
    g_signal_connect(button5, "clicked", G_CALLBACK(pixelate_filter_apply), image);
    g_signal_connect(button6, "clicked", G_CALLBACK(rgb_to_gray_filter_apply), image);
    g_signal_connect(button7, "clicked", G_CALLBACK(sepia_filter_apply), image);
    // append buttons to button box
    gtk_box_append(GTK_BOX(button_box0), button0);
    gtk_box_append(GTK_BOX(button_box0), button1);
    gtk_box_append(GTK_BOX(button_box0), button2);
    gtk_box_append(GTK_BOX(button_box0), button3);
    gtk_box_append(GTK_BOX(button_box1), button4);
    gtk_box_append(GTK_BOX(button_box1), button5);
    gtk_box_append(GTK_BOX(button_box1), button6);
    gtk_box_append(GTK_BOX(button_box1), button7);

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