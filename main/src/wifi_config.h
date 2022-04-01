#ifndef OPENPYROJET_WIFI_CONFIG_H
#define OPENPYROJET_WIFI_CONFIG_H

#include <stdbool.h>

#define WIFI_SSID_SIZE_LIMIT 32 // limited by wifi_sta_config_t in esp_wifi_types.h
#define WIFI_PASSWORD_SIZE_LIMIT 64 // limited by wifi_sta_config_t in esp_wifi_types.h

typedef struct {
    char ssid[WIFI_SSID_SIZE_LIMIT];
    char password[WIFI_PASSWORD_SIZE_LIMIT];
} WifiConfig;

bool wifi_config_is_usable(const WifiConfig* config);

#endif //OPENPYROJET_WIFI_CONFIG_H
