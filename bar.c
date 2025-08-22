#include "bar.h"
#include "parse.h"
#include "util.h"
#include <X11/Xatom.h>
#include <X11/Xft/Xft.h>

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
        strut[10] = bar->x;
        strut[11] = bar->width;
    }

    /* TODO: sticky, above */

    Atom _NET_WM_STRUT_PARTIAL = XInternAtom(bar->display, "_NET_WM_STRUT_PARITAL", 0);
    Atom _NET_WM_WINDOW_TYPE = XInternAtom(bar->display, "_NET_WM_WINDOW_TYPE", 0);
    Atom _NET_WM_WINDOW_TYPE_DOCK = XInternAtom(bar->display, "_NET_WM_WINDOW_TYPE_DOCK", 0);

    XChangeProperty(bar->display, bar->window, _NET_WM_WINDOW_TYPE, XA_ATOM, 32,
                    PropModeReplace, (unsigned char *)&_NET_WM_WINDOW_TYPE_DOCK, 1);
    XChangeProperty(bar->display, bar->window, _NET_WM_STRUT_PARTIAL, XA_CARDINAL, 32,
                    PropModeReplace, (unsigned char *)&strut, 12);
 }

Bar *
bar_create(Config *config)
{
    Bar *bar = malloc(sizeof(Bar));

    if (!(bar->display = XOpenDisplay(NULL)))
        panic("failed to open display, is X11 running?\n");

    /* TODO: check if this is fine */
    bar->screen = DefaultScreen(bar->display);
    bar->visual = DefaultVisual(bar->display, bar->screen);
    bar->depth = DefaultDepth(bar->display, bar->screen); 
    
    Window root = XRootWindow(bar->display, bar->screen);

    bar->colormap = XCreateColormap(bar->display, root,
                                    bar->visual,
                                    AllocNone);

    bar->font = XftFontOpenName(bar->display, bar->screen, config->font);

    XftColorAllocName(bar->display, bar->visual, bar->colormap,
                      config->background, &bar->background);
    XftColorAllocName(bar->display, bar->visual, bar->colormap,
                      config->foreground, &bar->foreground);
    
    XSetWindowAttributes setattributes = {
        .colormap = bar->colormap,
        .background_pixel = bar->background.pixel,
        .event_mask = ExposureMask
    };

    XWindowAttributes attributes;
    if (!XGetWindowAttributes(bar->display, root, &attributes))
        panic("failed to get window attributes for root window");
    
    bar->x = 0;
    bar->y = config->position == CONFIG_POSITION_TOP ? 0 :attributes.height - config->height;
    bar->width = attributes.width;
    bar->height = config->height;

    bar->window = XCreateWindow(bar->display, root, bar->x, bar->y,
                                bar->width, bar->height, 0,
                                CopyFromParent, CopyFromParent, CopyFromParent,
                                CWColormap | CWBackPixel | CWEventMask,
                                &setattributes);

    XStoreName(bar->display, bar->window, "par");
    XClassHint classhint = { "par", "par" };
    XSetClassHint(bar->display, bar->window, &classhint);

    bar->drawable = XCreatePixmap(bar->display, root, bar->width, bar->height,
                                  bar->depth);
    bar->gc = XCreateGC(bar->display, bar->drawable, 0, NULL);

    setatoms(bar, config);

    XMapRaised(bar->display, bar->window);

    return bar;
}

void
bar_map(Bar *bar) {
    XCopyArea(bar->display, bar->drawable, bar->window, bar->gc,
              bar->x, bar->y, bar->width, bar->height, bar->x, bar->y);
}

void
bar_draw(Bar *bar, Config *config)
{
    XSetForeground(bar->display, bar->gc, bar->background.pixel);
    XFillRectangle(bar->display, bar->drawable, bar->gc, 0, 0, bar->width, bar->height);

    /* IT'S ALL TEMPORARY */
    XftDraw *draw = XftDrawCreate(bar->display, bar->drawable, bar->visual, bar->colormap);
   
    /* TODO: figure out why this prevents leaks as opposed to doing it in bar_create
       TODO: free */
    bar->font = XftFontOpenName(bar->display, bar->screen, config->font);

    int y = bar->height - (bar->height - (bar->font->ascent - bar->font->descent)) / 2;
    XftDrawStringUtf8(draw, &bar->foreground, bar->font, config->gaps / 2, y, (XftChar8 *)"Hello, world!", 13);

    XftDrawDestroy(draw);
}
