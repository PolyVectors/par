#ifndef _LEX_H
#define _LEX_H

#include <stdlib.h>

typedef enum {
    TOKEN_TYPE_L_BRACE,
    TOKEN_TYPE_R_BRACE,
    TOKEN_TYPE_L_BRACKET,
    TOKEN_TYPE_R_BRACKET,
    TOKEN_TYPE_NUMBER,
    TOKEN_TYPE_STRING,
    TOKEN_TYPE_COLON,
    TOKEN_TYPE_COMMA
} TokenType;

typedef struct {
    TokenType type;
    char value[81];
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
