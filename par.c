#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include "bar.h"
#include "util.h"

#ifndef VERSION
#define VERSION "0.1.0"
#endif

/* TODO: consolidate into just one print call?
   TODO: hop on godbolt and check if using fputs is worthwhile */
#define PANIC_USAGE(...) do { \
                            fputs("\x1b[1;91merror\x1b[0m: ", stdout); \
                            printf(__VA_ARGS__); \
                            usage(1); \
                        } while (0) \

static void
usage(int code)
{
    puts(
        "par, an elementary X11 status bar.\n"
        "\n"
        "Usage: par <option(s)>\n"
        "\t-h (--help)                Show this text\n"
        "\t-v (--version)             Display the version\n"
        "\t-c (--config) <path>       Override the default config");
    exit(code);
}

static void
run(Config *config)
{
    Bar *bar;
    bar = bar_create(config);

    /* TODO: exit gracefully upon CTRL-C to prevent leaks */
    for (;;) {
        XFlush(bar->display);
    }

    free(bar);
}

int
main(int argc, char **argv)
{
    if (argc <= 1)
        usage(0);
    Config *config = NULL;

    for (int i = 1; i < argc; i++) {
        if (config != NULL)
            PANIC_USAGE("expected no additional arguments after config.\n");

        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            if (argc != 2 )
                PANIC_USAGE("argument `%s` expects no arguments.\n", argv[i]);
            usage(0);
        } else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version")) {
            if (argc != 2 )
                PANIC_USAGE("argument `%s` expects no arguments.\n", argv[i]);
            printf("par @commit %s\n", VERSION);
        } else if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--config")) {
            if (i == argc - 1)
                PANIC_USAGE("expected path after config, got nothing.\n");
            config = config_new(argv[i + 1]);
            i += 1;
        } else
            PANIC_USAGE("invalid argument `%s`.\n", argv[i]);
    }
    if (config == NULL)
        return 0;

    run(config);
    free(config);

    return 0;
}
