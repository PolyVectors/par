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

    char line[81];
    int i = 0;

    Token *tokens = calloc(1, sizeof(Token));
    while(fgets(line, filelength, fd)) {
        /* TODO: make some tokens */
        i++;
    }

    if (fclose(fd) != 0)
        panic("failed to close file at path `%s`\n", path);

    return tokens;
}
