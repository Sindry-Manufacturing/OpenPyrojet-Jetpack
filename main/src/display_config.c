#include "display_config.h"

bool display_config_is_usable(const DisplayConfig* config) {
    return config->pinScl >= 0 &&
        config->pinSda >= 0 &&
        config->pixelWidth > 0 &&
        config->pixelHeight > 0;
}
