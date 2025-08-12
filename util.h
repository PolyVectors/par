#define PANIC(...) fputs("\x1b[1;91merror\x1b[0m: ", stdout); \
                   printf(__VA_ARGS__); \
                   exit(1);
