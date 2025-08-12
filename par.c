#include <X11/Xlib.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "config.h"
#include "util.h"

#ifndef VERSION
#define VERSION "0.1.0"
#endif

/* TODO: consolidate into just one print call?
   TODO: hop on godbolt and check if using fputs is worthwhile */
#define PANIC_USAGE(...) fputs("\x1b[1;91merror\x1b[0m: ", stdout); \
                         printf(__VA_ARGS__); \
                         usage(1);

static void
run(Config *config)
{

}

static void
usage(int code)
{
    printf(
        "par, a minimalist X11 status bar.\n"
        "\n"
        "Usage: par <option(s)>\n"
        "\t-h (--help)                Show this text\n"
        "\t-v (--version)             Display the version\n"
        "\t-c (--config) <path>       Override the default config\n");
    exit(code);
}

int
main(int argc, char **argv)
{
    if (argc <= 1)
        usage(0);

    Config *config = NULL;

    for (int i = 1; i < argc; i++) {
        /* TODO: figure out why i can't remove the braces from this if statement */
        if (config != NULL) {
            PANIC_USAGE("expected no additional arguments after config has already been set.\n");
        }

        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            if (argc != 2 ) {
                PANIC_USAGE("argument `%s` expects no arguments.\n", argv[i]);
            }
            usage(0);
        } else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version")) {
            if (argc != 2 ) {
                PANIC_USAGE("argument `%s` expects no arguments.\n", argv[i]);
            }
            printf("par v%s\n", VERSION);
        } else if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--config")) {
            if (i == argc - 1) {
                PANIC_USAGE("expected path after config, got nothing.\n");
            }
            config = config_parse(argv[i + 1]);
            i += 1;
        } else {
            PANIC_USAGE("invalid argument `%s`.\n", argv[i]);
        }
    }
    if (config == NULL) {
        return 0;
    }

    run(config);
    free(config);

    return 0;
}