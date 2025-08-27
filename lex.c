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
    if (pos + 1 >= filelength)
        panic("Unclosed string while lexing.\n");

    pos++;
    size_t startpos = pos;

    while (pos < filelength) {
        if (source[pos] == '"')
            break;
        pos++;
    }

    Token token = { .type = TT_String };
    strncpy(token.value, source + startpos, pos - startpos);
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
    pos--;

    Token token = { .type = TT_Number };
    strncpy(token.value, source + startpos, pos - startpos);
    tokens_push(tokens, token);
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
                .type = source[pos] == '{' ? TT_LCurly : TT_RCurly
            });
            break;
        case '[':
        case ']':
            tokens_push(&tokens, (Token){
                .type = source[pos] == '[' ? TT_LBracket : TT_RBracket
            });
            break;
        case ':':
            tokens_push(&tokens, (Token){ .type = TT_Colon });
            break;
        case ',':
            tokens_push(&tokens, (Token){ .type = TT_Comma });
            break;
        case '"':
            pushstring(&tokens);
            break;
        default:
            if (source[pos] >= '0' || source[pos] <= '9')
                pushnumber(&tokens);
            else
                panic("Unexpected character '%c' while lexing", source[pos]);
        }
        pos++;
    }

    free(source);
    return tokens;
}
