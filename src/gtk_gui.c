#include <gtk/gtk.h>
#include "filters/black_and_white_filter.c"
#include "filters/bright_filter.c"
#include "filters/dark_filter.c"
#include "filters/emboss_filter.c"
#include "filters/negative_filter.c"
#include "filters/pixelate_filter.c"
#include "filters/rgb_to_gray_filter.c"
#include "filters/sepia_filter.c"

char *INPUT_IMAGE_PATH = "assets/images/test_image.bmp";
char *OUTPUT_IMAGE_PATH = "assets/images/%s.bmp"; // %s for filter name and image format To be Defined
static GtkWidget *label2; // Updates Label Text With The Output Image Format

char *get_image_format(const char *image_path) {
    char *extension = strrchr(image_path, '.');
    if (extension != NULL) {
        // Ignore the Dot character
        extension += 1;
        // Convert to uppercase
        char *capitalized_extension = g_ascii_strup(extension, -1);
        // Create space at the end
        char *formatted_string = g_strdup_printf("%s ", capitalized_extension);
        g_free(capitalized_extension);

        return formatted_string;
    }
    return NULL;
}

static void black_and_white_filter_apply(GtkWidget *widget, gpointer data)
{
    GtkWidget *image = data;

    const char *filter_name = "black_and_white";
    char output_path[256];

    sprintf(output_path, OUTPUT_IMAGE_PATH, filter_name);
    //sprintf(output_path, OUTPUT_IMAGE_PATH, filter_name, "jpeg"); <- To define the image format

    black_and_white_filter(INPUT_IMAGE_PATH, output_path);
    gtk_image_set_from_file(GTK_IMAGE(image), output_path);

    // update the label widget text with the output image format
    char *image_format = get_image_format(output_path);
    gtk_label_set_text(GTK_LABEL(label2), image_format);

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

    char *image_format = get_image_format(output_path);
    gtk_label_set_text(GTK_LABEL(label2), image_format);

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

    char *image_format = get_image_format(output_path);
    gtk_label_set_text(GTK_LABEL(label2), image_format);

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

    char *image_format = get_image_format(output_path);
    gtk_label_set_text(GTK_LABEL(label2), image_format);

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

    char *image_format = get_image_format(output_path);
    gtk_label_set_text(GTK_LABEL(label2), image_format);

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
    
    char *image_format = get_image_format(output_path);
    gtk_label_set_text(GTK_LABEL(label2), image_format);

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

    char *image_format = get_image_format(output_path);
    gtk_label_set_text(GTK_LABEL(label2), image_format);

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

    char *image_format = get_image_format(output_path);
    gtk_label_set_text(GTK_LABEL(label2), image_format);

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
    GtkWidget *window, *box, *image,*arrow_image,*image_container,*overlay1,*label1, *overlay2,
        *button_box0, *button_box1;
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
    image_container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
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
    overlay1 = gtk_overlay_new();
    gtk_widget_set_halign(overlay1, GTK_ALIGN_END);
    gtk_widget_set_valign(overlay1, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(image_container), overlay1);

    label1 = gtk_label_new(get_image_format(INPUT_IMAGE_PATH));
    gtk_widget_set_halign(label1, GTK_ALIGN_END);
    gtk_widget_set_valign(label1, GTK_ALIGN_START);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay1), label1);

    arrow_image = gtk_image_new_from_file("assets/images/arrow.png");
    gtk_image_set_pixel_size(GTK_IMAGE(arrow_image), 250);
    gtk_widget_set_halign(arrow_image, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(arrow_image, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(image_container), arrow_image);

    image = gtk_image_new_from_file(INPUT_IMAGE_PATH);
    gtk_image_set_pixel_size(GTK_IMAGE(image), 250);
    gtk_widget_set_halign(image, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(image, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(image_container), image);

    // create overlay for text 2
    overlay2 = gtk_overlay_new();
    gtk_widget_set_halign(overlay2, GTK_ALIGN_END);
    gtk_widget_set_valign(overlay2, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(image_container), overlay2);

    // TODO - get current output
    label2 = gtk_label_new(get_image_format(OUTPUT_IMAGE_PATH));
    gtk_widget_set_halign(label2, GTK_ALIGN_END);
    gtk_widget_set_valign(label2, GTK_ALIGN_START);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay2), label2);

    // TODO temp, spacing between images and buttons
    gtk_widget_set_margin_bottom(image_container, 100);
    gtk_widget_set_margin_top(image_container, 100);

    // create new horizontal box container for buttons
    button_box0 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_halign(button_box0, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button_box0, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(box), button_box0);

    // create new horizontal box container for buttons
    button_box1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_halign(button_box1, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button_box1, GTK_ALIGN_CENTER);
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