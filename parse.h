#ifndef _PARSE_H
#define _PARSE_H

#include "lex.h"

typedef enum {
    MODULE_TEXT,
    MODULE_CLOCK,
} ModuleType;

typedef struct {
    ModuleType type;
    char *value;

    char *prefix, *suffix; /* nullable */
    unsigned long foreground, background; /* also nullable */
} Module;

typedef enum {
    CONFIG_POSITION_TOP,
    CONFIG_POSITION_BOTTOM,
} ConfigPosition;

typedef struct {
    char font[81];
    char foreground[8]; /* default value */
    char background[8];

    ConfigPosition position;
    unsigned int height;
    unsigned int gaps;

    char separator;

    Module *left;
    Module *right;
} Config;

Config *parse_config(Tokens tokens);

#endif
