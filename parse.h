#ifndef _PARSE_H
#define _PARSE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "lex.h"

#define CONFIG_POSITION_TOP (unsigned char) 0
#define CONFIG_POSITION_BOTTOM (unsigned char) 1

typedef enum {
    M_TEXT,
    M_CLOCK,
} ModuleType;

typedef struct {
    ModuleType type;
    char *value;

    char *prefix, *suffix; /* nullable */
    unsigned long foreground, background; /* also nullable */
} Module;

typedef struct {
    const char *font;
    const char *foreground; /* default value */
    const char *background;

    unsigned char position;
    unsigned int height;
    unsigned int gaps;

    char separator;

    Module *left;
    Module *right;
} Config;

Config *parse_config(Token *tokens);

#endif
