#include "config.h"

typedef struct {
    Display *display;
    Window window;
    unsigned int x, y, width, height;
} Bar;

Bar *bar_create(Config *config);