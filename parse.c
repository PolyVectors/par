#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"
#include "lex.h"
#include "util.h"

#define FONT_DEFINITION          (1 << 0)
#define BACKGROUND_DEFINITION    (1 << 1)
#define FOREGROUND_DEFINITION    (1 << 2)
#define POSITION_DEFINITION      (1 << 3)
#define HEIGHT_DEFINITION        (1 << 4)
#define GAPS_DEFINITION          (1 << 5)
#define SEPARATOR_DEFINITION     (1 << 6)

#define DEFINITIONS              7
static unsigned char defined = 0;

static size_t pos = 0;

static void
parse_array()
{
    
}


/* TODO: split this up into smaller individual functions, e.g. parse_object_root */
static void
parse_object(Tokens tokens, Config *config)
{
    unsigned char root = pos == 0;
    pos++;

    while (pos < tokens.count) {
        switch (tokens.array[pos].type) {
        case TT_LCurly:
            parse_object(tokens, config);
            break;
        case TT_RCurly:
            return;
            break;
        case TT_String:
            if (root) {
                char *identifier = tokens.array[pos].value;
                char *value = tokens.array[pos + 2].value;
                
                if (!strcmp(identifier, "font")) {
                    strcpy(config->font, value);
                    defined |= FONT_DEFINITION;
                } else if (!strcmp(identifier, "foreground")
                         || !strcmp(identifier, "background")
                ) {
                    if (strlen(value) != 7
                        || tokens.array[pos + 2].type != TT_String)
                        panic("identifier \"%s\" requires a hexadecimal string (i.e. #FFFFFF), got '%s'.\n",
                              identifier, value);

                    int foreground = !strcmp(identifier, "foreground");

                    strcpy(foreground ? config->foreground
                                      : config->background, value);
                    defined |= foreground ? FOREGROUND_DEFINITION
                                          : BACKGROUND_DEFINITION;
                } else if (!strcmp(identifier, "position")) {
                    if (strcmp(value, "top") && strcmp(value, "bottom"))
                        panic("identifier \"position\" expects a string either \"top\" or \"bottom\", got '%s'.\n",
                              value);
                    config->position = !strcmp(value, "bottom")
                                        ? CONFIG_POSITION_BOTTOM
                                        : CONFIG_POSITION_TOP;
                    defined |= POSITION_DEFINITION;
                } else if (!strcmp(identifier, "height")
                           || !strcmp(identifier, "gaps")
                ) {
                    if (tokens.array[pos + 2].type != TT_Number)
                        panic("identifier \"height\" expects a number, got '%s'.\n",
                              value);

                    if (!strcmp(identifier, "height")) {
                        config->height = atoi(value);
                        defined |= HEIGHT_DEFINITION;
                    } else {
                        config->gaps = atoi(value);
                        defined |= GAPS_DEFINITION;
                    }

                } else if (!strcmp(identifier, "separator")) {
                    if (tokens.array[pos + 2].type != TT_String
                        || strlen(value) != 1)
                        panic("identifier \"separator\" expects a single character, got '%s'.\n",
                              value);
                    config->separator = value[0];
                    defined |= SEPARATOR_DEFINITION;
                } else if (!strcmp(identifier, "left-modules")
                           || !strcmp(identifier, "right-modules"))
                {
                    /* TODO: parse modules */
                } else {
                    panic("unknown identifier '%s'\n", identifier);
                }
                
                pos += tokens.array[pos + 3].type != TT_RCurly ? 3 : 2;
            }
            break;
        default:
            printf("unexpected token type '%d'\n", tokens.array[pos].type);
            break;
        }
        
        pos++;
    }

    panic("expected closing brace \"}\" while parsing config, got nothing.\n");
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

    int i = DEFINITIONS;
    while (i--) {
        if (((1 << i) & defined) == (1 << i))
            continue;

        switch ((1 << i)) {
        case FONT_DEFINITION:
            strcpy(config->font, "monospace:size=8");
            break;
        case BACKGROUND_DEFINITION:
            strcpy(config->background, "#FFFFFF");
            break;
        case FOREGROUND_DEFINITION:
            strcpy(config->foreground, "#000000");
            break;
        case POSITION_DEFINITION:
            config->position = CONFIG_POSITION_TOP;
            break;
        case HEIGHT_DEFINITION:
            config->height = 16;
            break;
        case GAPS_DEFINITION:
            config->gaps = 2;
            break;
        case SEPARATOR_DEFINITION:
            config->separator = '|';
            break;
        }
    }

    config->left = NULL;
    config->right = NULL;

    return config;
}
