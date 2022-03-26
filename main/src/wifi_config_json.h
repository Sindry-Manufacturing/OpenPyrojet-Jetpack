#ifndef OPENPYROJET_WIFI_CONFIG_JSON_H
#define OPENPYROJET_WIFI_CONFIG_JSON_H

#include <cJSON.h>
#include <stdbool.h>
#include "wifi_config.h"

bool wifi_config_from_json(const cJSON* json, WifiConfig* wifiConfig);

void wifi_config_to_json(cJSON* root, const WifiConfig* config);
void wifi_config_to_json_safe(cJSON* root, const WifiConfig* config);

#endif //OPENPYROJET_WIFI_CONFIG_JSON_H
