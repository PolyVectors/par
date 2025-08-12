#include "time.h"

#define CONFIG_POSITION_TOP (unsigned char)0
#define CONFIG_POSITION_BOTTOM (unsigned char)1

typedef enum {
    MODULE_TEXT,
    MODULE_CLOCK,
} ModuleType;

typedef union {
    char *text; /* MODULE_TEXT data */
    time_t time; /* MODULE_CLOCK data */
} ModuleData;

typedef struct {
    ModuleType type;
    ModuleData data;

    char *prefix, *suffix;
    unsigned long foreground, background;
} Module;

typedef struct {
    unsigned char position;

    int padding;
    char separator;
    Module *left;
    Module *right;
} Config;

Config *config_parse(const char *path);