#include <gtk/gtk.h>
#include <gdk/gdkx.h> // Needed for X11 hints

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GTK Panel");

    // Remove window decorations (borderless)
    gtk_window_set_decorated(GTK_WINDOW(window), FALSE);

    // Always on top
    gtk_window_set_keep_above(GTK_WINDOW(window), TRUE);

    // Set size and position
    gtk_window_set_default_size(GTK_WINDOW(window), 1920, 30); // width, height
    gtk_window_move(GTK_WINDOW(window), 0, 0); // position at top

    // Set window type hint to "dock"
    gtk_window_set_type_hint(GTK_WINDOW(window), GDK_WINDOW_TYPE_HINT_DOCK);

    // Optional: background color
    GdkRGBA color;
    gdk_rgba_parse(&color, "#222222");
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
        "* { background-color: #222222; color: #ffffff; }",
        -1, NULL);

    GtkStyleContext *context = gtk_widget_get_style_context(window);
    gtk_style_context_add_provider(context,
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // Content: just a label for now
    GtkWidget *label = gtk_label_new("GTK Panel");
    gtk_container_add(GTK_CONTAINER(window), label);

    // Exit cleanly
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
