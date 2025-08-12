#define CONFIG_POSITION_TOP (char)0
#define CONFIG_POSITION_BOTTOM (char)1

typedef struct {
    char position;
    // TODO: other options here
} Config;

Config *config_parse(const char *path);