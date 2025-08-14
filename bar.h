#include "config.h"

typedef struct {
    unsigned int x, y, w, h;
    
    Display *dpy;
    XWindowAttributes wa;
    Window win;
} Bar;

Bar *bar_create(Config *config);