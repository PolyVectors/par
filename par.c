#include <X11/Xlib.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "config.h"

// TODO: consolidate into just one print call?
// TODO: hop on godbolt and check if using fputs is worthwhile
#define ERROR_USAGE(...) \
                    fputs("\x1b[1;91merror\x1b[0m: ", stdout); \
                    printf(__VA_ARGS__); \
                    usage();

static const char *usagetext =
    "par, a minimalist X11 status bar.\n"
    "\n"
    "Usage: par <option(s)>\n"
    "\t-h (--help)        Show this text\n"
    "\t-c (--config)      Override the default config\n";

static void
usage(void)
{
    printf(usagetext);
    exit(0);
}

int
main(int argc, char **argv)
{
    Config *config = NULL;

    if (argc <= 1)
        usage();

    for (int i = 1; i < argc; i++) {
        // TODO: figure out why i can't remove the braces from this if statement
        if (config != NULL) {
            ERROR_USAGE("additional arguments after config has already been set.\n");
        }

        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            if (argc != 2 ) {
                ERROR_USAGE("argument `%s` expects nothing.\n", argv[i]);
            }
            usage();
        } else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--config") == 0) {
            if (i == argc - 1) {
                ERROR_USAGE("expected path after config, got nothing.\n");
            }
            config = config_parse(argv[i + 1]);
            i += 1;
        } else {
            ERROR_USAGE("invalid argument `%s`.\n", argv[i]);
        }
    }

    free(config);
    return 0;
}