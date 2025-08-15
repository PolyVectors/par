#include "time.h"

#define CONFIG_POSITION_TOP (unsigned char)0
#define CONFIG_POSITION_BOTTOM (unsigned char)1

typedef enum {
    MODULE_TEXT,
    MODULE_CLOCK,
} ModuleType;

typedef union {
    char *text;
} ModuleData;

typedef struct {
    ModuleType type;
    ModuleData data;

    char *prefix, *suffix; /* nullable */
    unsigned long foreground, background; /* also nullable */
} Module;

typedef struct {
    unsigned char position;
    unsigned int height;
    unsigned int gaps;

    unsigned long background;
    char separator;

    Module *left;
    Module *right;
} Config;

Config *config_new(const char *path);