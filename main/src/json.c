#include "json.h"

#include <esp_log.h>

static const char* TAG = "json";

bool json_object_get_property_as_integer(const cJSON* json, const char* key, int* number) {
    const cJSON* item = cJSON_GetObjectItemCaseSensitive(json, key);
    if (cJSON_IsNull(item) || !cJSON_IsNumber(item)) {
        ESP_LOGE(TAG, "%s is not a number", key);
        return false;
    } else {
        *number = item->valueint;
        return true;
    }
}
