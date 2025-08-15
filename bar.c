#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include "bar.h"
#include "util.h"

/* https://specifications.freedesktop.org/wm-spec/1.3/ar01s05.html */
static void
setatoms(Bar *bar, Config *config)
{
    long strut[12] = {0};

    if (config->position == CONFIG_POSITION_TOP) {
        strut[2] = bar->height;
        strut[8] = bar->x;
        strut[9] = bar->x + bar->width - 1;
    } else {
        strut[3] = bar->height;
        strut[10] = 0;
        strut[11] = bar->width;
    }

    /* TODO: sticky, above */

    Atom _NET_WM_STRUT = XInternAtom(bar->display, "_NET_WM_STRUT", 0);
    Atom _NET_WM_STRUT_PARTIAL = XInternAtom(bar->display, "_NET_WM_STRUT", 0);
    Atom _NET_WM_WINDOW_TYPE = XInternAtom(bar->display, "_NET_WM_WINDOW_TYPE", 0);
    Atom _NET_WM_WINDOW_TYPE_DOCK = XInternAtom(bar->display, "_NET_WM_WINDOW_TYPE_DOCK", 0);
    Atom _MOTIF_WM_HINTS = XInternAtom(bar->display, "_MOTIF_WM_HINTS", 0);

    XChangeProperty(bar->display, bar->window, _NET_WM_WINDOW_TYPE, XA_ATOM, 32,
                    PropModeReplace, (unsigned char *)&_NET_WM_WINDOW_TYPE_DOCK, 1);

    long prop[5] = {2, 0, 0, 0, 0};
    XChangeProperty(bar->display, bar->window, _MOTIF_WM_HINTS, _MOTIF_WM_HINTS, 32,
                    PropModeReplace, (unsigned char *)prop, 5);

    XChangeProperty(bar->display, bar->window, _NET_WM_STRUT_PARTIAL, XA_CARDINAL, 32,
                    PropModeReplace, (unsigned char *)&strut, 12);
    XChangeProperty(bar->display, bar->window, _NET_WM_STRUT, XA_CARDINAL, 32,
                    PropModeReplace, (unsigned char *)&strut, 4);
}

Bar *
bar_create(Config *config)
{
    Bar *bar = malloc(sizeof(Bar));

    if (!(bar->display = XOpenDisplay(NULL)))
        panic("failed to open display, is X11 running?\n");

    int screen = DefaultScreen(bar->display);
    Window root = XRootWindow(bar->display, screen);

    Colormap colormap = XCreateColormap(bar->display, root,
                                        DefaultVisual(bar->display, screen),
                                        AllocNone);

    /* TODO: gc, pixmap */
    XSetWindowAttributes setattributes = {
        .override_redirect = 1,
        .colormap = colormap,
        .background_pixel = config->background,
        .event_mask = ExposureMask | KeyPressMask | VisibilityChangeMask,
    };

    XWindowAttributes attributes;
    if (!XGetWindowAttributes(bar->display, root, &attributes))
        panic("failed to get window attributes for root window");
    
    bar->x = 0;
    bar->y = config->position ? attributes.height - config->height : 0;
    bar->width = attributes.width;
    bar->height = config->height;

    bar->window = XCreateWindow(bar->display, root, bar->x, bar->y,
                                bar->width, bar->height, 0,
                                CopyFromParent, CopyFromParent, CopyFromParent,
                                CWBackPixel | CWBorderPixel | CWEventMask | CWColormap,
                                &setattributes);;

    setatoms(bar, config);

    XMapWindow(bar->display, bar->window);
    XFlush(bar->display);
    XMapRaised(bar->display, bar->window);

    return bar;
}
