#include "bar.h"

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <X11/Xft/Xft.h>

#ifndef VERSION
#define VERSION "unknown"
#endif

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
panicusage(const char *fmt, ...)
{
    va_list args;
    va_start (args, fmt);

    fputs("\x1b[1;91merror\x1b[0m: ", stderr);
    vfprintf(stderr, fmt, args);
    usage(1);
}

static void
run(Config *config)
{
    Bar *bar = bar_create(config);

    time_t last = 0;
    XEvent ev;

    for (;;) {
        while (XPending(bar->display)) {
            XNextEvent(bar->display, &ev);
            if (XFilterEvent(&ev, bar->window))
                continue;
        }

        time_t now = time(NULL);
        if (now - last >= 1) {
            bar_draw(bar, config);
            bar_map(bar);
            last = now;
        }

        nanosleep(&(struct timespec){0, 100000000}, NULL);
    }

    /* TODO: never reached, fix */
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
                panicusage("expected no additional arguments after config.\n");
        else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            if (argc != 2 )
                panicusage("argument `%s` expects no arguments.\n", argv[i]);
            usage(0);
        } else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version")) {
            if (argc != 2 )
                panicusage("argument `%s` expects no arguments.\n", argv[i]);
            printf("par git-%s\n", VERSION);
        } else if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--config")) {
            if (i == argc - 1)
                panicusage("expected path after config, got nothing.\n");
            config = config_parse(argv[i + 1]);
            i += 1;
        } else
            panicusage("invalid argument `%s`.\n", argv[i]);
    }
    if (config == NULL)
        return 0;

    run(config);
 
    /* TODO: this is ALSO never reached, ditto */
    free(config);

    return 0;
}
