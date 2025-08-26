#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"
#include "lex.h"
#include "util.h"

static size_t pos = 0;

static void
parse_object(Tokens tokens, Config *config)
{
    unsigned char root = 0;
    if (pos == 0)
        root = 1;
    
    while (pos < tokens.count) {
        switch (tokens.array[pos].type) {
        case TT_LCurly:
            pos++;
            parse_object(tokens, config);
            break;
        case TT_String:
            if (pos + 2 < tokens.count
                && tokens.array[pos + 1].type == TT_Colon
                && (tokens.array[pos + 2].type == TT_String
                    || tokens.array[pos + 2].type == TT_Number
                    || tokens.array[pos + 2].type == TT_LBracket)
            ) {
                char *identifier = tokens.array[pos].value;
                char *value = tokens.array[pos + 2].value;
                
                if (!strcmp(identifier, "font")) 
                    strcpy(config->font, value);
                else if (!strcmp(identifier, "foreground")
                         || !strcmp(identifier, "background")
                ) {
                    if (strlen(value) != 7)
                        panic("identifier \"%s\" requires a hexadecimal string (i.e. #FFFFFF), got `%s`.\n",
                              identifier, value);
                    strcpy(!strcmp(identifier, "foreground")
                            ? config->foreground
                            : config->background, value);
                } else if (!strcmp(identifier, "position")) {
                    if (strcmp(value, "top") && strcmp(value, "bottom"))
                        panic("identifier \"position\" expects either a string top or bottom, got `%s`.\n",
                              value);
                    config->position = !strcmp(value, "bottom")
                                        ? CONFIG_POSITION_BOTTOM
                                        : CONFIG_POSITION_TOP;
                } else if (!strcmp(identifier, "height")) {
                    if (tokens.array[pos + 2].type != TT_Number)
                        panic("identifier \"height\" expects a number, got `%s`.\n",
                              value);
                    config->height = atoi(value);
                }
                // TODO: gaps, separator, left-modules, right-modules
            }
            break;
        default:
            break;
        }
        
        pos++;
    }
}
    
Config *
parse_config(Tokens tokens)
{
    Config *config = calloc(1, sizeof(Config));

    while (pos < tokens.count) {
        switch (tokens.array[pos].type) {
            case TT_LCurly:
                parse_object(tokens, config);
                break;
            default:
                break;
        }
        pos++;
    }

    /* placeholder */
    if (!config->font[0])
        strcpy(config->font, "monospace:size=8");

    if (!config->foreground[0])
        strcpy(config->foreground, "#FFFFFF");
    if (!config->background[0])
        strcpy(config->background, "#000000");

    if (!config->height)
        config ->height = 16;

    config->gaps = 4;
    config->separator = '|';

    config->left = NULL;
    config->right = NULL;

    return config;
}
