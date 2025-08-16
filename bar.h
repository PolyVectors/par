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
