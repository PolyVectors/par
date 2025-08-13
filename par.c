#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include "config.h"
#include "util.h"

#ifndef VERSION
#define VERSION "0.1.0"
#endif

/* TODO: consolidate into just one print call?
   TODO: hop on godbolt and check if using fputs is worthwhile */
#define PANIC_USAGE(...) fputs("\x1b[1;91merror\x1b[0m: ", stdout); \
                         printf(__VA_ARGS__); \
                         usage(1);

static void
usage(int code)
{
    puts(
        "par, an elementary X11 status bar.\n"
        "\n"
        "Usage: par <option(s)>\n"
        "\t-h (--help)                Show this text\n"
        "\t-v (--version)             Display the version\n"
        "\t-c (--config) <path>       Override the default config");
    exit(code);
}

static void
atoms(Display *display, Window win)
{
    /* TODO: fix and tidy up, but more importantly, fix (maybe try strut)
       https://x.org/releases/X11R7.6/doc/xorg-docs/specs/ICCCM/icccm.html */
       
    Atom NET_WM_WINDOW_TYPE =  XInternAtom(display, "_NET_WM_WINDOW_TYPE", 0);
    Atom NET_WM_STATE = XInternAtom(display, "_NET_WM_STATE", 0);
    Atom NET_WM_WINDOW_TYPE_DOCK = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DOCK", 0);
    Atom NET_WM_STATE_STICKY = XInternAtom(display, "_NET_WM_STATE_STICKY", 0);

    XChangeProperty(display, win, NET_WM_WINDOW_TYPE, XA_ATOM, 32, 1, (const unsigned char *)&NET_WM_WINDOW_TYPE_DOCK, 1);
    XChangeProperty(display, win, NET_WM_STATE, XA_ATOM, 32, 2, (const unsigned char *)&NET_WM_STATE_STICKY, 1);
    XChangeProperty(display, win, XA_WM_NAME, XA_STRING, 8, 3, "par", 1);
    XChangeProperty(display, win, XA_WM_CLASS, XA_STRING, 8, 7, "par\0Bar", 1);
}

static void
run(Config *config)
{
    Display *display;
    if (!(display = XOpenDisplay(NULL)))
        panic("failed to open display, is X11 running?\n");

    int screen = DefaultScreen(display);
    Window root = XRootWindow(display, screen);

    XSetWindowAttributes windowattributes;
    windowattributes.override_redirect = 1;
    windowattributes.background_pixel = 0x00000000;
    windowattributes.event_mask = ExposureMask | KeyPressMask | VisibilityChangeMask;

    /* TODO: pixmap and gc */
    Window win = XCreateWindow(display, root, 0, 1440 - 24, 2560, 24, 0,
                                CopyFromParent, CopyFromParent, CopyFromParent,
                                CWBackPixel | CWBorderPixel | CWOverrideRedirect
                                | CWEventMask | CWColormap,
                                &windowattributes);
    atoms(display, win);

    XMapWindow(display, win);
    XFlush(display);
    XMapRaised(display, win);

    for (;;) { }
}

int
main(int argc, char **argv)
{
    if (argc <= 1)
        usage(0);

    Config *config = NULL;

    for (int i = 1; i < argc; i++) {
        if (config != NULL) {
            PANIC_USAGE("expected no additional arguments after config.\n");
        }

        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            if (argc != 2 ) {
                PANIC_USAGE("argument `%s` expects no arguments.\n", argv[i]);
            }
            usage(0);
        } else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version")) {
            if (argc != 2 ) {
                PANIC_USAGE("argument `%s` expects no arguments.\n", argv[i]);
            }
            printf("par @commit %s\n", VERSION);
        } else if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--config")) {
            if (i == argc - 1) {
                PANIC_USAGE("expected path after config, got nothing.\n");
            }
            config = config_new(argv[i + 1]);
            i += 1;
        } else {
            PANIC_USAGE("invalid argument `%s`.\n", argv[i]);
        }
    }
    if (config == NULL) {
        return 0;
    }

    run(config);
    free(config);

    return 0;
}