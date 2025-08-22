#include "parse.h"
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

Config *
config_parse(const char *path)
{
    FILE *fd = fopen(path, "r");
    if (fd == NULL)
        panic("file at path `%s` doesn't exist.\n", path);

    fseek(fd, 0, SEEK_END);
    long filelength = ftell(fd) + 1;
    fseek(fd, 0, SEEK_SET);

    char line[81];
    int i = 0;

    while(fgets(line, filelength, fd)) {
        if (whitespace(line))
            continue;

        /* TODO: validate */
        i++;
    }

    if (fclose(fd) != 0)
        panic("failed to close file at path `%s`\n", path);

    Config *config = malloc(sizeof(Config));
 
    /* placeholder */
    config->font = "Fairfax:size=8";
    
    config->foreground = "#FFFFFF";
    config->background = "#000000";

    /* TODO: fix no rendering w/ bottom config */
    config->position = CONFIG_POSITION_BOTTOM;
    config ->height = 16;
    config->gaps = 4;

    config->separator = '|';
    config->left = NULL;
    config->right = NULL;

    return config;
}
