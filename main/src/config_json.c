#include "config_json.h"

#include <esp_log.h>

#include "wifi_config_json.h"

static const char* TAG = "config-json";

bool config_read_nozzle_pins(Config* config, const cJSON* jsonArray) {
    if (!cJSON_IsArray(jsonArray)) {
        ESP_LOGE(TAG, "nozzlePins is not an array");
        return false;
    }
    config->nozzleCount = cJSON_GetArraySize(jsonArray);
    int nozzleIndex = 0;
    cJSON* jsonPin = NULL;
    cJSON_ArrayForEach(jsonPin, jsonArray)
    {
        int pinNumber = jsonPin->valueint;
        config->nozzlePins[nozzleIndex] = pinNumber;
        ESP_LOGI(TAG, "nozzle %d: pin %d", nozzleIndex, pinNumber);
        nozzleIndex++;
    }
    for (; nozzleIndex < (config->nozzleCount - 1); nozzleIndex++) {
        config->nozzlePins[nozzleIndex] = 0; // set the undefined pins to 0
    }

    return true;
}

bool config_from_json(Config* config, const cJSON* json) {
    const cJSON* wifiJson = cJSON_GetObjectItemCaseSensitive(json, "wifi");
    if (cJSON_IsObject(wifiJson)) {
        if (!wifi_config_from_json(wifiJson, &(config->wifi))) {
            return false;
        }
    }
    const cJSON* heatingDurationJson = cJSON_GetObjectItemCaseSensitive(json, "heatingDuration");
    if (heatingDurationJson != NULL) {
        config->heatingDuration = heatingDurationJson->valueint;
        ESP_LOGI(TAG, "heatingDuration = %d µs", config->heatingDuration);
    }

    const cJSON* triggerDelayJson = cJSON_GetObjectItemCaseSensitive(json, "triggerDelay");
    if (triggerDelayJson != NULL) {
        config->triggerDelay = triggerDelayJson->valueint;
        ESP_LOGI(TAG, "triggerDelay = %d µs", config->triggerDelay);
    }

    const cJSON* nozzlePinsJson = cJSON_GetObjectItemCaseSensitive(json, "nozzlePins");
    if (nozzlePinsJson != NULL) {
        if (!config_read_nozzle_pins(config, nozzlePinsJson)) {
            return false;
        }
        ESP_LOGI(TAG, "found %d nozzle pins", config->nozzleCount);
    }

    return true;
}

bool config_from_json_text(Config* config, const char* jsonBuffer) {
    cJSON* json = cJSON_Parse(jsonBuffer);
    bool result = config_from_json(config, json);
    cJSON_Delete(json);
    return result;
}

static cJSON* config_to_json_internal(const Config* config, bool removeRiskItems) {
    cJSON* root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "heatingDuration", config->heatingDuration);
    cJSON_AddNumberToObject(root, "triggerDelay", config->triggerDelay);

    // Collect the nozzle pins as an int[]
    int pinInts[NOZZLE_COUNT_MAX];
    int currentIndex = 0;
    while (currentIndex < NOZZLE_COUNT_MAX) {
        uint8 pin = config->nozzlePins[currentIndex];
        if (pin > 0) {
            pinInts[currentIndex] = (int) pin;
            currentIndex++;
        } else {
            break;
        }
    }

    cJSON* nozzlePins = cJSON_CreateIntArray(pinInts, currentIndex);
    cJSON_AddItemToObject(root, "nozzlePins", nozzlePins);

    cJSON* wifi = cJSON_AddObjectToObject(root, "wifi");
    if (removeRiskItems) {
        wifi_config_to_json_safe(wifi, &config->wifi);
    } else {
        wifi_config_to_json(wifi, &config->wifi);
    }

    return root;
}

cJSON* config_to_json_safe(const Config* config) {
    return config_to_json_internal(config, true);
}

cJSON* config_to_json(const Config* config) {
    return config_to_json_internal(config, false);
}

