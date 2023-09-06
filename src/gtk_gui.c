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
    // include box container in window
    gtk_window_set_child(GTK_WINDOW(window), box);

    // create a horizontal box container for the images
    GtkWidget *image_container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_halign(image_container, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(image_container, GTK_ALIGN_CENTER);
    // append the image container to the main vertical box
    gtk_box_append(GTK_BOX(box), image_container);

    // Create a vertical box container for the user image
    user_image_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(user_image_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(user_image_box, GTK_ALIGN_CENTER);
    // append the user image container to the horizontal image container
    gtk_box_append(GTK_BOX(image_container), user_image_box);

    // create image widget for user image
    image = gtk_image_new_from_file("assets/images/test_image.bmp");
    gtk_image_set_pixel_size(GTK_IMAGE(image), 250);
    // set horizontal alignment to center for the image
    gtk_widget_set_halign(image, GTK_ALIGN_CENTER);
    // append the user image widget to the user image container
    gtk_box_append(GTK_BOX(user_image_box), image);

    // set a top margin for the user image to raise it within the layout
    gtk_widget_set_margin_top(image, 50);

    GtkWidget *new_image = gtk_image_new_from_file("assets/images/arrow.png");
    gtk_image_set_pixel_size(GTK_IMAGE(new_image), 250);
    gtk_widget_set_halign(new_image, GTK_ALIGN_CENTER);

    // create a new container for the new image
    GtkWidget *new_image_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(new_image_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(new_image_box, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(image_container), new_image_box);

    // append the new image widget to the new image container
    gtk_box_append(GTK_BOX(new_image_box), new_image);
    gtk_widget_set_margin_top(new_image, 50);

    // create a vertical box container for the filtered image
    filtered_image_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(filtered_image_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(filtered_image_box, GTK_ALIGN_CENTER);
    // append the filtered image container to the horizontal image container
    gtk_box_append(GTK_BOX(image_container), filtered_image_box);

    // create image widget for filtered image
    image = gtk_image_new_from_file("assets/images/test_image.bmp");
    gtk_image_set_pixel_size(GTK_IMAGE(image), 250);
    // set horizontal alignment to center for the image
    gtk_widget_set_halign(image, GTK_ALIGN_CENTER);
    // append the filtered image widget to the filtered image container
    gtk_box_append(GTK_BOX(filtered_image_box), image);

    // set a top margin for the filtered image to raise it within the layout
    gtk_widget_set_margin_top(image, 50);

    // create new horizontal box container for buttons
    button_box0 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_halign(button_box0, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button_box0, GTK_ALIGN_CENTER);
    // append button box to vertical box
    gtk_box_append(GTK_BOX(box), button_box0);
    gtk_widget_set_margin_top(button_box0, 100); // temp 

    // create new horizontal box container for buttons
    button_box1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_halign(button_box1, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button_box1, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_bottom(button_box1, 20);
    // append button box to vertical box
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

    // show window
    gtk_widget_set_visible(window, TRUE);
}