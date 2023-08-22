#include "gtk_gui.c"

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