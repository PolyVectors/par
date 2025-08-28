#include <stdio.h>
#include <string.h>

#include "lex.h"
#include "util.h"

static void
tokens_init(Tokens *tokens)
{
    tokens->array = malloc(sizeof(Token));
    tokens->capacity = 1;
    tokens->count = 0;
}

static void
tokens_push(Tokens *tokens, Token token)
{
    if (tokens->capacity >= tokens->count) {
        tokens->capacity += 5;
        tokens->array = realloc(tokens->array,
                                tokens->capacity * sizeof(Token));
    }
    tokens->array[tokens->count++] = token;
}

void
lex_tokens_free(Tokens *tokens)
{
    free(tokens->array);
    tokens->array = NULL;
    tokens->capacity = tokens->count = 0;
}

static long filelength;
static char *source;
static size_t pos;

static void
pushstring(Tokens *tokens)
{
    /* TODO: a segfault can occur with unclosed strings, fix */
    if (pos + 1 >= filelength)
        panic("unclosed string while lexing.\n");

    pos++;
    size_t startpos = pos;

    while (pos < filelength) {
        if (source[pos] == '"')
            break;
        pos++;
    }

    Token token = { .type = TOKEN_TYPE_STRING };
    strncpy(token.value, source + startpos, pos - startpos);
    
    TokenType previous = tokens->array[tokens->count - 1].type;
    if (previous != TOKEN_TYPE_COMMA
        && previous != TOKEN_TYPE_COLON
        && previous != TOKEN_TYPE_L_BRACE
    )
        panic("expected \",\" or \":\" before string '%s', got '%s'.\n",
              token.value, tokens->array[tokens->count - 1].value);

    tokens_push(tokens, token);
}

static void
pushnumber(Tokens *tokens)
{
    size_t startpos = pos;
    while (pos < filelength) {
        if (source[pos] > '9' || source[pos] < '0')
            break;
        pos++;
    }

    Token token = { .type = TOKEN_TYPE_NUMBER };
    strncpy(token.value, source + startpos, pos - startpos);
    
    if (tokens->array[tokens->count - 1].type != TOKEN_TYPE_COLON)
        panic("expected \",\" or \":\" before number '%s', got '%s'.\n",
              token.value, tokens->array[tokens->count - 1].value);
    
    tokens_push(tokens, token);
    pos--;
}

Tokens
lex_config(const char *path)
{
    FILE *fd = fopen(path, "r");
    if (fd == NULL)
        panic("file at path `%s` doesn't exist.\n", path);

    fseek(fd, 0, SEEK_END);
    filelength = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    source = calloc(filelength, sizeof(char));
    fread(source, sizeof(char), filelength, fd);

    if (fclose(fd) != 0)
        panic("failed to close file at path `%s`\n", path);

    Tokens tokens;
    tokens_init(&tokens);
    
    while (pos < filelength) {
        switch (source[pos]) {
        case ' ':
        case '\t':
        case '\n':
            break;
        case '{':
        case '}':
            tokens_push(&tokens, (Token){
                .type = source[pos] == '{' ? TOKEN_TYPE_L_BRACE
                                           : TOKEN_TYPE_R_BRACE,
                .value = source[pos]
            });
            break;
        case '[':
        case ']':
            tokens_push(&tokens, (Token){
                .type = source[pos] == '[' ? TOKEN_TYPE_L_BRACKET
                                           : TOKEN_TYPE_R_BRACKET,
                .value = source[pos]
            });
            break;
        case ':':
            if (tokens.array[tokens.count - 1].type != TOKEN_TYPE_STRING)
                panic("expected a string before \":\", got `%s`.\n",
                      tokens.array[tokens.count - 1]);
            
            tokens_push(&tokens,
                        (Token){ .type = TOKEN_TYPE_COLON, .value = ":" });
            break;
        case ',':
            tokens_push(&tokens,
                        (Token){ .type = TOKEN_TYPE_COMMA, .value = "," });
            break;
        case '"':
            pushstring(&tokens);
            break;
        default:
            if (source[pos] >= '0' || source[pos] <= '9')
                pushnumber(&tokens);
            else
                panic("unexpected character '%c' while lexing", source[pos]);
        }
        pos++;
    }

    if (tokens.array[tokens.count - 1].type != TOKEN_TYPE_R_BRACE)
        panic("expected config to end in \"}\", got '%s'.\n",
              tokens.array[tokens.count - 1].value);
    if (tokens.array[tokens.count - 2].type == TOKEN_TYPE_COMMA)
        panic("found trailing comma in config while lexing.\n");

    free(source);
    return tokens;
}
