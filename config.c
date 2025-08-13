#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "config.h"
#include "util.h"

static unsigned char
whitespace(const char *str)
{
    size_t i = strlen(str);
    while (i--) {
        if (str[i] != '\t' && str[i] != '\n' && str[i] != ' ')
            return 0;
    }
    return 1;
}

static unsigned char
indents(const char *str)
{
    unsigned char indentation = 0;
    for (size_t i = 0; i < strlen(str); i++)
        if (str[i] == '\t' || str[i] == '\n' || str[i] == ' ')
            indentation++;
        else
            return indentation;
    return indentation;
}

Config *
config_new(const char *path)
{
    FILE *fd;
    fd = fopen(path, "r");
    if (fd == NULL) {
        panic("file at path `%s` doesn't exist.\n", path);
    }

    fseek(fd, 0, SEEK_END);
    long filelength = ftell(fd) + 1; /* TODO: figure out why +1 is needed? */
    fseek(fd, 0, SEEK_SET);

    char line[81];
    int i = 0;

    while(fgets(line, filelength, fd)) {
        if (whitespace(line))
            continue;

        unsigned char indentation = indents(line);
        (void)(indentation);

        /* TODO: validate */
        i++;
    }
    
    if (fclose(fd) != 0) {
        panic("failed to close file at path `%s`\n", path);
    }

    Config *config = malloc(sizeof(Config));

    /* placeholder */
    config->position = CONFIG_POSITION_TOP;

    config->gaps = 0;
    config->separator = '|';
    config->left = NULL;
    config->right = NULL;

    return config;
}