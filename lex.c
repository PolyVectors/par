#include "lex.h"

static void
tokens_create(Tokens *tokens)
{
    tokens->array = malloc(sizeof(Token));
    tokens->capacity = 1;
    tokens->count = 0;
}

static void
tokens_push(Tokens *tokens, Token token)
{
    if (tokens->capacity >= tokens->count) {
        tokens->capacity *= 2;
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

static size_t pos = 0;

Tokens
lex_config(const char *path)
{
    FILE *fd = fopen(path, "r");
    if (fd == NULL)
        panic("file at path `%s` doesn't exist.\n", path);

    fseek(fd, 0, SEEK_END);
    long filelength = ftell(fd) + 1;
    fseek(fd, 0, SEEK_SET);

    char *source = calloc(filelength, sizeof(char));
    fread(source, sizeof(char), filelength, fd);

    if (fclose(fd) != 0)
        panic("failed to close file at path `%s`\n", path);
    
    Tokens tokens;
    tokens_create(&tokens);
    
    while (pos < filelength) {
        switch (source[pos]) {
        case ' ':
        case '\t':
        case '\n':
            break;
        case '{':
            tokens_push(&tokens, (Token){ .type = TT_LCurly });
            break;
        case '}':
            tokens_push(&tokens, (Token){ .type = TT_RCurly });
            break;
        /* TODO: more cases */
        default:
            /* this will be a panic when the lexer is finished */
            printf("Unrecognized character: `%c` while lexing\n", source[pos]);
            break;
        }

        pos++;
    }

    free(source);
    return tokens;
}
