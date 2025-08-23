#include "parse.h"

Config *
parse_config(Token *tokens)
{
    Config *config = malloc(sizeof(Config));
 
    /* placeholder */
    config->font = "Fairfax:size=8";
    
    config->foreground = "#FFFFFF";
    config->background = "#000000";

    config->position = CONFIG_POSITION_BOTTOM;
    config ->height = 16;
    config->gaps = 4;

    config->separator = '|';
    config->left = NULL;
    config->right = NULL;

    return config;
}
