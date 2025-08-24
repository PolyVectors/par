#ifndef _LEX_H
#define _LEX_H

#include <stdlib.h>

typedef enum {
    TT_LCurly,
    TT_RCurly,
    TT_String
} TokenType;

typedef struct {
    TokenType type;
    char value[80];
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
