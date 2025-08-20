#ifndef _BAR_H
#define _BAR_H

#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/Xft/Xft.h>

#include "parse.h"

typedef struct {
    XftFont *font;

    XftColor foreground;
    XftColor background;

    Display *display;
    int screen;
    int depth;
    Visual *visual;
    Window window;

    Colormap colormap;
    Pixmap drawable;
    GC gc;

    unsigned int x, y, width, height;
} Bar;

Bar *bar_create(Config *config);
void bar_draw(Bar *bar, Config *config);
void bar_map(Bar *bar);

#endif
