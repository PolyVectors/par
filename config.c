#include "config.h"

#include <stdio.h>
#include <stdlib.h>

Config *
config_parse(const char *path)
{
    // TODO: validate path

    Config *config = malloc(sizeof(Config));
    // TODO: initialize here
    config->position = 0; // placeholder
    return config;
}