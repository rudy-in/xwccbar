#define GDK_WINDOWING_WAYLAND
#define GDK_WINDOWING_X11
#include <gtk/gtk.h>

#ifdef GDK_WINDOWING_X11
#include <gdk/gdkx.h>
#include <X11/Xatom.h>
#endif

#ifdef GDK_WINDOWING_WAYLAND
#include <gdk/gdkwayland.h>
#include <gtk-layer-shell/gtk-layer-shell.h>
#endif

#include <iostream>

// cli parser
#include "cli/parser.hpp"

// create window
#include "window/creation.hpp"

#include "config/loader.hpp"

int main(int argc, char *argv[]) {
    cli::Args args = cli::parse(argc, argv);

    auto window_cfg = config::load_config(args.config_path);

    // create window (gtk)
    create_gtk_window(&argc, &argv, window_cfg);

    return 0;
}
