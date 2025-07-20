#include "config.hpp"
#include <X11/Xlib.h> // for X11 screen dimensions
#include <string>
#include <algorithm> // for std::transform

void set_x11_dock_hint(GtkWidget *window) {
#ifdef GDK_WINDOWING_X11
    GdkWindow *gdk_window = gtk_widget_get_window(window);
    if (!GDK_IS_X11_WINDOW(gdk_window)) return;

    Display *xdisplay = GDK_WINDOW_XDISPLAY(gdk_window);
    Window xwindow = GDK_WINDOW_XID(gdk_window);

    Atom window_type_atom = XInternAtom(xdisplay, "_NET_WM_WINDOW_TYPE", False);
    Atom dock_atom = XInternAtom(xdisplay, "_NET_WM_WINDOW_TYPE_DOCK", False);
    XChangeProperty(xdisplay, xwindow, window_type_atom, XA_ATOM, 32,
                    PropModeReplace, (unsigned char *) &dock_atom, 1);
#endif
}

std::string normalize_position(const std::string& pos) {
    std::string norm = pos;
    std::transform(norm.begin(), norm.end(), norm.begin(), ::tolower);
    return norm;
}

int create_gtk_window(int* argc, char*** argv, const WindowConfig& cfg) {
    gtk_init(argc, argv);

    std::string position = normalize_position(cfg.position);
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GTK Panel");
    gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    GdkDisplay *display = gdk_display_get_default();
    gboolean is_wayland = GDK_IS_WAYLAND_DISPLAY(display);
    gboolean is_x11 = GDK_IS_X11_DISPLAY(display);

    if (is_wayland) {
#ifdef GDK_WINDOWING_WAYLAND
        gtk_layer_init_for_window(GTK_WINDOW(window));
        gtk_layer_set_layer(GTK_WINDOW(window), GTK_LAYER_SHELL_LAYER_TOP);
        gtk_layer_auto_exclusive_zone_enable(GTK_WINDOW(window));

        // Clear all anchors first
        gtk_layer_set_anchor(GTK_WINDOW(window), GTK_LAYER_SHELL_EDGE_TOP, FALSE);
        gtk_layer_set_anchor(GTK_WINDOW(window), GTK_LAYER_SHELL_EDGE_BOTTOM, FALSE);
        gtk_layer_set_anchor(GTK_WINDOW(window), GTK_LAYER_SHELL_EDGE_LEFT, FALSE);
        gtk_layer_set_anchor(GTK_WINDOW(window), GTK_LAYER_SHELL_EDGE_RIGHT, FALSE);

        if (position == "top") {
            gtk_layer_set_anchor(GTK_WINDOW(window), GTK_LAYER_SHELL_EDGE_TOP, TRUE);
            gtk_layer_set_anchor(GTK_WINDOW(window), GTK_LAYER_SHELL_EDGE_LEFT, TRUE);
            gtk_layer_set_anchor(GTK_WINDOW(window), GTK_LAYER_SHELL_EDGE_RIGHT, TRUE);
            gtk_window_set_default_size(GTK_WINDOW(window), 1, 30);
        } else if (position == "bottom") {
            gtk_layer_set_anchor(GTK_WINDOW(window), GTK_LAYER_SHELL_EDGE_BOTTOM, TRUE);
            gtk_layer_set_anchor(GTK_WINDOW(window), GTK_LAYER_SHELL_EDGE_LEFT, TRUE);
            gtk_layer_set_anchor(GTK_WINDOW(window), GTK_LAYER_SHELL_EDGE_RIGHT, TRUE);
            gtk_window_set_default_size(GTK_WINDOW(window), 1, 30);
        } else {
            // fallback
            gtk_layer_set_anchor(GTK_WINDOW(window), GTK_LAYER_SHELL_EDGE_TOP, TRUE);
            gtk_layer_set_anchor(GTK_WINDOW(window), GTK_LAYER_SHELL_EDGE_LEFT, TRUE);
            gtk_layer_set_anchor(GTK_WINDOW(window), GTK_LAYER_SHELL_EDGE_RIGHT, TRUE);
            gtk_window_set_default_size(GTK_WINDOW(window), 1, 30);
        }
#endif
    } if (is_x11) {
        gtk_window_set_type_hint(GTK_WINDOW(window), GDK_WINDOW_TYPE_HINT_DOCK);
        gtk_window_set_skip_taskbar_hint(GTK_WINDOW(window), TRUE);
        gtk_window_set_skip_pager_hint(GTK_WINDOW(window), TRUE);
        gtk_window_set_keep_above(GTK_WINDOW(window), TRUE);

        // Use GDK's X11 connection (avoid XOpenDisplay)
        GdkDisplay *gdk_display = gdk_display_get_default();
        Display *xdisplay = GDK_DISPLAY_XDISPLAY(gdk_display);
        Screen *screen = DefaultScreenOfDisplay(xdisplay);
        int screen_width = screen->width;
        int screen_height = screen->height;

        if (cfg.position == "top") {
            gtk_window_set_default_size(GTK_WINDOW(window), screen_width, 30);
            gtk_window_move(GTK_WINDOW(window), 0, 0);
        } else if (cfg.position == "bottom") {
            gtk_window_set_default_size(GTK_WINDOW(window), screen_width, 30);
            gtk_window_move(GTK_WINDOW(window), 0, screen_height - 30);
        } else {
            // fallback
            gtk_window_set_default_size(GTK_WINDOW(window), screen_width, 30);
            gtk_window_move(GTK_WINDOW(window), 0, 0);
        }

        g_signal_connect(window, "realize", G_CALLBACK(set_x11_dock_hint), NULL);
    }


    // CSS
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
        "* { background-color: #222222; color: #ffffff; }", -1, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(window);
    gtk_style_context_add_provider(context,
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // Content
    GtkWidget *label = gtk_label_new("GTK Panel");
    gtk_container_add(GTK_CONTAINER(window), label);

    // Clean exit
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    gtk_main();
    
    return 0;
}
