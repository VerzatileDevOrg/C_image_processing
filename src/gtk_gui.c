#include <gtk/gtk.h>

#include "filters/black_and_white_filter.c"
#include "filters/bright_filter.c"
#include "filters/dark_filter.c"
#include "filters/emboss_filter.c"
#include "filters/negative_filter.c"
#include "filters/pixelate_filter.c"
#include "filters/rgb_to_gray_filter.c"
#include "filters/sepia_filter.c"

const char *INPUT_IMAGE_PATH = "assets/images/test_image.bmp";
const char *OUTPUT_IMAGE_PATH = "assets/images/%s.bmp";

static void black_and_white_filter_apply(GtkWidget *widget, gpointer data)
{
    GtkWidget *image = data;

    const char *filter_name = "black_and_white";
    char output_path[256];
    sprintf(output_path, OUTPUT_IMAGE_PATH, filter_name);

    black_and_white_filter(INPUT_IMAGE_PATH, output_path);
    gtk_image_set_from_file(GTK_IMAGE(image), output_path);
    g_print("Black And White Filter Has Been Applied\n");
}

static void bright_filter_apply(GtkWidget *widget, gpointer data)
{
    GtkWidget *image = data;

    const char *filter_name = "bright";
    char output_path[256];
    sprintf(output_path, OUTPUT_IMAGE_PATH, filter_name);

    bright_filter(INPUT_IMAGE_PATH, output_path);
    gtk_image_set_from_file(GTK_IMAGE(image), output_path);
    g_print("Bright Filter Has Been Applied\n");
}

static void dark_filter_apply(GtkWidget *widget, gpointer data)
{
    GtkWidget *image = data;

    const char *filter_name = "dark";
    char output_path[256];
    sprintf(output_path, OUTPUT_IMAGE_PATH, filter_name);

    dark_filter(INPUT_IMAGE_PATH, output_path);
    gtk_image_set_from_file(GTK_IMAGE(image), output_path);
    g_print("Dark Filter Has Been Applied\n");
}

static void emboss_filter_apply(GtkWidget *widget, gpointer data)
{
    GtkWidget *image = data;

    const char *filter_name = "emboss";
    char output_path[256];
    sprintf(output_path, OUTPUT_IMAGE_PATH, filter_name);

    emboss_filter(INPUT_IMAGE_PATH, output_path);
    gtk_image_set_from_file(GTK_IMAGE(image), output_path);
    g_print("Emboss Filter Has Been Applied\n");
}

static void negative_filter_apply(GtkWidget *widget, gpointer data)
{
    GtkWidget *image = data;

    const char *filter_name = "negative";
    char output_path[256];
    sprintf(output_path, OUTPUT_IMAGE_PATH, filter_name);

    negative_filter(INPUT_IMAGE_PATH, output_path);
    gtk_image_set_from_file(GTK_IMAGE(image), output_path);
    g_print("Negative Filter Has Been Applied\n");
}

static void pixelate_filter_apply(GtkWidget *widget, gpointer data)
{
    GtkWidget *image = data;

    const char *filter_name = "pixelate";
    char output_path[256];
    sprintf(output_path, OUTPUT_IMAGE_PATH, filter_name);

    pixelate_filter(INPUT_IMAGE_PATH, output_path);
    gtk_image_set_from_file(GTK_IMAGE(image), output_path);
    g_print("Pixelate Filter Has Been Applied\n");
}

static void rgb_to_gray_filter_apply(GtkWidget *widget, gpointer data)
{
    GtkWidget *image = data;

    const char *filter_name = "rgb_to_gray";
    char output_path[256];
    sprintf(output_path, OUTPUT_IMAGE_PATH, filter_name);

    rgb_to_gray_filter(INPUT_IMAGE_PATH, output_path);
    gtk_image_set_from_file(GTK_IMAGE(image), output_path);
    g_print("RGB To Gray Filter Has Been Applied\n");
}

static void sepia_filter_apply(GtkWidget *widget, gpointer data)
{
    GtkWidget *image = data;

    const char *filter_name = "sepia";
    char output_path[256];
    sprintf(output_path, OUTPUT_IMAGE_PATH, filter_name);

    sepia_filter(INPUT_IMAGE_PATH, output_path);
    gtk_image_set_from_file(GTK_IMAGE(image), output_path);
    g_print("Sepia Filter Has Been Applied\n");
}

// struct for filter buttons
typedef struct
{
    const char *label;
    void (*callback)(GtkWidget *, gpointer);
} FilterButtonInfo;

// connect filter buttons to callback functions
void connect_filter_buttons(GtkWidget *image, const FilterButtonInfo *button_info, int num_buttons, GtkWidget *button_box)
{
    for (int i = 0; i < num_buttons; ++i)
    {
        GtkWidget *button = gtk_button_new_with_label(button_info[i].label);
        g_signal_connect(button, "clicked", G_CALLBACK(button_info[i].callback), image);
        gtk_box_append(GTK_BOX(button_box), button);
    }
}

static void activate(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window, *box, *image, *user_image_box, *filtered_image_box,
        *button_box0, *button_box1, *button0, *button1, *button2,
        *button3, *button4, *button5, *button6, *button7;
    const char *css = "window {"
                        "    background-color: #FFFFE4;"
                        "}"
                        "headerbar.titlebar {"
                        "    font-family: 'BigNoodleTitling', 'Sans';"
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
                        "    border: 2px solid #008CBA;"
                        "    padding: 10px 20px;"
                        "    text-decoration: none;"
                        "    font-size: 22px;"
                        "    margin: 8px 5px;"
                        "    font-family: 'BigNoodleTitling', 'Sans';"
                        "}";
    FilterButtonInfo filter_buttons[] = {
        {"Black And White Filter", black_and_white_filter_apply},
        {"Bright Filter", bright_filter_apply},
        {"Dark Filter", dark_filter_apply},
        {"Emboss Filter", emboss_filter_apply},
        {"Negative Filter", negative_filter_apply},
        {"Pixelate Filter", pixelate_filter_apply},
        {"RGB To Gray Filter", rgb_to_gray_filter_apply},
        {"Sepia Filter", sepia_filter_apply}};
    int num_filter_buttons = sizeof(filter_buttons) / sizeof(filter_buttons[0]);

    // create new window
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "C Image Processing");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    // create new vertical box container
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
    gtk_window_set_child(GTK_WINDOW(window), box);

    // create a horizontal box container for the images
    GtkWidget *image_container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_halign(image_container, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(image_container, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(box), image_container);

    // create image widget for user image 1
    image = gtk_image_new_from_file(INPUT_IMAGE_PATH);
    gtk_image_set_pixel_size(GTK_IMAGE(image), 250);
    gtk_widget_set_halign(image, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(image, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(image_container), image);

    // create overlay for text 1
    GtkWidget *overlay1 = gtk_overlay_new();
    gtk_widget_set_halign(overlay1, GTK_ALIGN_END);
    gtk_widget_set_valign(overlay1, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(image_container), overlay1);

    // TODO - get image type and place it to label
    GtkWidget *label1 = gtk_label_new("BMP ");
    gtk_widget_set_halign(label1, GTK_ALIGN_END);
    gtk_widget_set_valign(label1, GTK_ALIGN_START);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay1), label1);

    GtkWidget *new_image = gtk_image_new_from_file("assets/images/arrow.png");
    gtk_image_set_pixel_size(GTK_IMAGE(new_image), 250);
    gtk_widget_set_halign(new_image, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(new_image, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(image_container), new_image);

    // create image widget for user image 3 TEMP before output image
    image = gtk_image_new_from_file(INPUT_IMAGE_PATH);
    gtk_image_set_pixel_size(GTK_IMAGE(image), 250);
    gtk_widget_set_halign(image, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(image, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(image_container), image);

    // create overlay for text 2
    GtkWidget *overlay2 = gtk_overlay_new();
    gtk_widget_set_halign(overlay2, GTK_ALIGN_END);
    gtk_widget_set_valign(overlay2, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(image_container), overlay2);

    // TODO - get image type and place it to label
    GtkWidget *label2 = gtk_label_new("BMP ");
    gtk_widget_set_halign(label2, GTK_ALIGN_END);
    gtk_widget_set_valign(label2, GTK_ALIGN_START);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay2), label2);


    // create new horizontal box container for buttons
    button_box0 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_halign(button_box0, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button_box0, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(box), button_box0);
    gtk_widget_set_margin_top(button_box0, 100); // TODO spacing from images 

    // create new horizontal box container for buttons
    button_box1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_halign(button_box1, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button_box1, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_bottom(button_box1, 20);
    gtk_box_append(GTK_BOX(box), button_box1);

    // connect filter buttons to callback functions
    connect_filter_buttons(image, filter_buttons, num_filter_buttons / 2, button_box0);
    connect_filter_buttons(image, filter_buttons + num_filter_buttons / 2, num_filter_buttons / 2, button_box1);

    // create new CSS provider object and load CSS
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(provider, css);
    // add CSS provider to default display
    gtk_style_context_add_provider_for_display(
    gdk_display_get_default(),
    GTK_STYLE_PROVIDER(provider),
    GTK_STYLE_PROVIDER_PRIORITY_USER);

    gtk_widget_set_visible(window, TRUE);
}