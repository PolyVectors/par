#include "time.h"

/* TODO: try casting enum to char */
#define CONFIG_POSITION_TOP (unsigned char)0
#define CONFIG_POSITION_BOTTOM (unsigned char)1

typedef enum {
    MODULE_TEXT,
    MODULE_CLOCK,
} ModuleType;

typedef union {
    char *text; /* MODULE_TEXT data */
} ModuleData;

typedef struct {
    ModuleType type;
    ModuleData data;

    char *prefix, *suffix; /* if we are doing systray stuff, these can be null */
    unsigned long foreground, background;
} Module;

typedef struct {
    unsigned char position;

    int gaps;
    char separator;

    Module *left;
    Module *right;
} Config;

Config *config_new(const char *path);