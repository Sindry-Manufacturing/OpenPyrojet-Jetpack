#ifndef OPENPYROJET_JSON_H
#define OPENPYROJET_JSON_H

#include <cJSON.h>
#include <stdbool.h>

bool json_object_get_property_as_integer(const cJSON* json, const char* key, int* number);

#endif //OPENPYROJET_JSON_H
