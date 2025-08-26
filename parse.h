#ifndef _PARSE_H
#define _PARSE_H

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
    char font[81];
    char foreground[8]; /* default value */
    char background[8];

    unsigned char position;
    unsigned int height;
    unsigned int gaps;

    char separator;

    Module *left;
    Module *right;
} Config;

Config *parse_config(Tokens tokens);

#endif
