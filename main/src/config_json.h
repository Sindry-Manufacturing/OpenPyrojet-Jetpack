#ifndef OPENPYROJET_CONFIG_JSON_H
#define OPENPYROJET_CONFIG_JSON_H

#include <cJSON.h>
#include <stdbool.h>
#include "config.h"

bool config_from_json_text(Config* config, const char* jsonBuffer);
bool config_from_json(Config* config, const cJSON* json);

cJSON* config_to_json(const Config* config);

/**
 * Same as config_to_json(), but without risk items like the WiFi password.
 */
cJSON* config_to_json_safe(const Config* config);

bool config_save(const Config* config);

#endif //OPENPYROJET_CONFIG_JSON_H
