#ifndef _BAR
#define _BAR

#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#include "config.h"

typedef struct {
    Display *display;
    Window window;
    Pixmap drawable;
    GC gc;
    unsigned int x, y, width, height;
} Bar;

Bar *bar_create(Config *config);
void bar_draw(Bar *bar, Config *config);
void bar_map(Bar *bar);

#endif
