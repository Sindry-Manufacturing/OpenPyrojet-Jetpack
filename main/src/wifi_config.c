#include "wifi_config.h"

#include <string.h>

bool wifi_config_is_usable(const WifiConfig* config) {
    return strlen(config->ssid) > 0;
}
