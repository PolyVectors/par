#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include "bar.h"
#include "util.h"

/* TODO: fix
   https://specifications.freedesktop.org/wm-spec/1.3/ar01s05.html */
static void
setatoms(Bar *bar, Config *config)
{
    long strut[12] = {0};

    if (config->position == CONFIG_POSITION_TOP) {
        strut[2] = bar->h;
        strut[8] = bar->x;
        strut[9] = bar->x + bar->w - 1;
    } else {
        strut[3] = bar->h;
        strut[10] = 0;
        strut[11] = bar->wa.width;
    }

    Atom _NET_WM_STRUT = XInternAtom(bar->dpy, "_NET_WM_STRUT", 0);
    Atom _NET_WM_STRUT_PARTIAL = XInternAtom(bar->dpy, "_NET_WM_STRUT", 0);
    Atom _NET_WM_WINDOW_TYPE = XInternAtom(bar->dpy, "_NET_WM_WINDOW_TYPE", 0);
    Atom _NET_WM_WINDOW_TYPE_DOCK = XInternAtom(bar->dpy, "_NET_WM_WINDOW_TYPE_DOCK", 0);
    Atom _MOTIF_WM_HINTS = XInternAtom(bar->dpy, "_MOTIF_WM_HINTS", 0);

    XChangeProperty(bar->dpy, bar->win, _NET_WM_WINDOW_TYPE, XA_ATOM, 32,
                    PropModeReplace, (unsigned char *)&_NET_WM_WINDOW_TYPE_DOCK, 1);

    long prop[5] = {2, 0, 0, 0, 0};
    XChangeProperty(bar->dpy, bar->win, _MOTIF_WM_HINTS, _MOTIF_WM_HINTS, 32,
                    PropModeReplace, (unsigned char *)prop, 5);

    XChangeProperty(bar->dpy, bar->win, _NET_WM_STRUT_PARTIAL, XA_CARDINAL, 32,
                    PropModeReplace, (unsigned char *)&strut, 12);
    XChangeProperty(bar->dpy, bar->win, _NET_WM_STRUT, XA_CARDINAL, 32,
                    PropModeReplace, (unsigned char *)&strut, 4);
}

Bar *
bar_create(Config *config)
{
    Bar *bar = malloc(sizeof(Bar));

    Display *dpy;
    if (!(dpy = XOpenDisplay(NULL)))
        panic("failed to open display, is X11 running?\n");

    int screen = DefaultScreen(dpy);
    Window root = XRootWindow(dpy, screen);

    XSetWindowAttributes swa;
    swa.override_redirect = 1;
    swa.background_pixel = 0x00000000;
    swa.event_mask = ExposureMask | KeyPressMask | VisibilityChangeMask;

    XWindowAttributes wa;
    if (!XGetWindowAttributes(dpy, root, &wa))
        panic("failed to get window attributes for root window");
    
    bar->x = 0;
    bar->y = config->position ? wa.height - config->height : 0;
    bar->w = wa.width;
    bar->h = config->height;

    Window win = XCreateWindow(dpy, root, bar->x, bar->y, bar->w, bar->h, 0,
                                CopyFromParent, CopyFromParent, CopyFromParent,
                                CWBackPixel | CWBorderPixel | CWEventMask | CWColormap,
                                &swa);

    bar->dpy = dpy;
    bar->win = win;
    bar->wa = wa;

    setatoms(bar, config);

    /* TODO: configure window, pixmap, gc */

    XMapWindow(bar->dpy, bar->win);
    XFlush(bar->dpy);
    XMapRaised(bar->dpy, bar->win);

    return bar;
}