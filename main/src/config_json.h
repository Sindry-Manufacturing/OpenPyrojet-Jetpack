#ifndef OPENPYROJET_CONFIG_JSON_H
#define OPENPYROJET_CONFIG_JSON_H

#include <cJSON.h>
#include <stdbool.h>
#include "config.h"

bool config_from_json(Config* config, const char* jsonBuffer);

cJSON* config_to_json(const Config* config);

#endif //OPENPYROJET_CONFIG_JSON_H
