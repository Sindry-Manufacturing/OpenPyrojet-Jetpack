#ifndef OPENPYROJET_DISPLAY_CONFIG_H
#define OPENPYROJET_DISPLAY_CONFIG_H

#include <stdbool.h>

typedef struct {
    int pinScl;
    int pinSda;
    int pixelWidth;
    int pixelHeight;
} DisplayConfig;

bool display_config_is_usable(const DisplayConfig* config);

#endif //OPENPYROJET_DISPLAY_CONFIG_H
