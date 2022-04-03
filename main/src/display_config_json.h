#ifndef OPENPYROJET_DISPLAY_CONFIG_JSON_H
#define OPENPYROJET_DISPLAY_CONFIG_JSON_H

#include <cJSON.h>
#include <stdbool.h>

#include "display_config.h"

bool display_config_from_json(const cJSON* json, DisplayConfig* config);
void display_config_to_json(cJSON* root, const DisplayConfig* config);

#endif //OPENPYROJET_DISPLAY_CONFIG_JSON_H
