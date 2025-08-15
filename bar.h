#include "config.h"

typedef struct {
    Display *display;
    XWindowAttributes attributes;
    Window window;
    unsigned int x, y, width, height;
} Bar;

Bar *bar_create(Config *config);