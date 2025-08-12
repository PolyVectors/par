#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "config.h"
#include "util.h"

Config *
config_parse(const char *path)
{
    FILE *fd;
    fd = fopen(path, "r");
    if (fd == NULL) {
        PANIC("file at path `%s` doesn't exist.\n", path);
    }

    /* TODO: read into a buffer and process i.e. fseek, etc.
       TODO: toml? ron? kdl? json? yaml? ini? idk */

    if (fclose(fd) != 0) {
        PANIC("failed to close file at path `%s`\n", path);
    }

    Config *config = malloc(sizeof(Config));

    /* placeholder */
    config->position = CONFIG_POSITION_TOP;

    config->padding = 0;
    config->separator = '|';
    config->left = NULL;
    config->right = NULL;

    return config;
}