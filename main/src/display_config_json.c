#include "display_config_json.h"

#include "json.h"

bool display_config_from_json(const cJSON* json, DisplayConfig* config) {
    int pinScl, pinSda, pixelWidth, pixelHeight;
    bool readAll = json_object_get_property_as_integer(json, "pinScl", &pinScl) &&
        json_object_get_property_as_integer(json, "pinSda", &pinSda) &&
        json_object_get_property_as_integer(json, "pixelWidth", &pixelWidth) &&
        json_object_get_property_as_integer(json, "pixelHeight", &pixelHeight);

    if (readAll) {
        config->pinSda = pinSda;
        config->pinScl = pinScl;
        config->pixelHeight = pixelHeight;
        config->pixelWidth = pixelWidth;
    }

    return readAll;
}

void display_config_to_json(cJSON* json, const DisplayConfig * config) {
    cJSON_AddNumberToObject(json, "pinScl", config->pinScl);
    cJSON_AddNumberToObject(json, "pinSda", config->pinSda);
    cJSON_AddNumberToObject(json, "pixelWidth", config->pixelWidth);
    cJSON_AddNumberToObject(json, "pixelHeight", config->pixelHeight);
}
