#ifndef _LEX_H
#define _LEX_H

#include <stdio.h>
#include <stdlib.h>

#include "util.h"

typedef enum {
    TT_LCurly,
    TT_RCurly,
} TokenType;

typedef struct {
    TokenType type;
    char *value;
} Token;

Token *lex_config(const char *parse);

#endif
