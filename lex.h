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

/* https://x.com/tsoding/status/1898807595168575589 */
typedef struct {
    Token *array;
    size_t capacity;
    size_t count;
} Tokens;

void lex_tokens_free(Tokens *tokens);
Tokens lex_config(const char *parse);

#endif
