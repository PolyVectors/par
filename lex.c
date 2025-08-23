#include "lex.h"

Token *
lex_config(const char *path)
{
    FILE *fd = fopen(path, "r");
    if (fd == NULL)
        panic("file at path `%s` doesn't exist.\n", path);

    fseek(fd, 0, SEEK_END);
    long filelength = ftell(fd) + 1;
    fseek(fd, 0, SEEK_SET);

    char *source = calloc(filelength + 1, sizeof(char));
    fread(source, sizeof(char), filelength, fd);
    
    Token *tokens = malloc(sizeof(Token));
    size_t i = 0;
    
    while (i < filelength) {
        switch (source[i]) {
        /* TODO: add cases for tokens */
        case ' ':
        case '\t':
        case '\n':
            break;
        }

        i++;
    }
    
    if (fclose(fd) != 0)
        panic("failed to close file at path `%s`\n", path);

    free(source);
    return tokens;
}
