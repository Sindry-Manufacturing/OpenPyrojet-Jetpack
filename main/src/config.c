#include "config.h"
#include "esp_log.h"
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"

#include "file.h"

static const char* TAG = "config";
static const char* userConfigPath = "/config/user.json";
static const char* defaultConfigPath = "/config/default.json";

Config config;

esp_err_t config_read_nozzle_pins(Config* config, const cJSON* jsonArray) {
    if (!cJSON_IsArray(jsonArray)) {
        ESP_LOGE(TAG, "nozzlePins is not an array");
        return ESP_FAIL;
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

    return ESP_OK;
}

esp_err_t config_wifi_get(const cJSON* root, WifiConfig* wifiConfig) {
    wifiConfig->ssid[0] = 0;
    wifiConfig->password[0] = 0;

    const cJSON* wifiJson = cJSON_GetObjectItemCaseSensitive(root, "wifi");
    if (cJSON_IsNull(wifiJson)) {
        ESP_LOGE(TAG, "wifi config not found");
        return ESP_FAIL;
    }
    const cJSON* ssidJson = cJSON_GetObjectItemCaseSensitive(wifiJson, "ssid");
    if (cJSON_IsNull(ssidJson) || !cJSON_IsString(ssidJson)) {
        ESP_LOGE(TAG, "wifi ssid is not a string");
        return ESP_FAIL;
    }

    const cJSON* passwordJson = cJSON_GetObjectItemCaseSensitive(wifiJson, "password");
    if (cJSON_IsNull(passwordJson) || !cJSON_IsString(passwordJson)) {
        ESP_LOGE(TAG, "wifi password is not a string");
        return ESP_FAIL;
    }

    const char* ssid = cJSON_GetStringValue(ssidJson);
    const char* password = cJSON_GetStringValue(passwordJson);

    strcpy(wifiConfig->ssid, ssid);
    strcpy(wifiConfig->password, password);

    return ESP_OK;
}

esp_err_t config_from_json(Config* config, const char* jsonBuffer) {
    cJSON* root = cJSON_Parse(jsonBuffer);

    config_wifi_get(root, &(config->wifi));

    config->heatingDuration = cJSON_GetObjectItemCaseSensitive(root, "heatingDuration")->valueint;
    ESP_LOGI(TAG, "heatingDuration = %d µs", config->heatingDuration);

    config->triggerDelay = cJSON_GetObjectItemCaseSensitive(root, "triggerDelay")->valueint;
    ESP_LOGI(TAG, "triggerDelay = %d µs", config->triggerDelay);

    const cJSON* nozzlePinsJson = cJSON_GetObjectItemCaseSensitive(root, "nozzlePins");
    if (config_read_nozzle_pins(config, nozzlePinsJson) != ESP_OK) {
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "found %d nozzle pins", config->nozzleCount);

    cJSON_Delete(root);
    return ESP_OK;
}

bool config_ensure_user_copy() {
    if (file_exists(userConfigPath)) { // file exists
        ESP_LOGI(TAG, "using existing %s", userConfigPath);
        return true;
    } else {
        return file_copy(defaultConfigPath, userConfigPath);
    }
}

esp_err_t config_init() {
    if (!config_ensure_user_copy()) {
        ESP_LOGE(TAG, "failed to ensure existence of %s", userConfigPath);
        return false;
    }

    const char* userConfigPath = "/config/user.json";
    FILE* file = fopen(userConfigPath, "r");
    if (file == NULL) {
        ESP_LOGE(TAG, "failed to open file %s", userConfigPath);
        return false;
    }

    // Verify file size
    long fileSize = file_get_size(file);
    if (fileSize > CONFIG_FILE_SIZE_MAX) {
        ESP_LOGE(TAG, "file too large: %ld bytes", fileSize);
        return ESP_FAIL;
    }

    char buffer[fileSize + 1]; // needs space for null terminator
    size_t bytesRead = fread(buffer, 1, fileSize, file);
    if (ferror(file) != 0) {
        ESP_LOGE(TAG, "read error");
        fclose(file);
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "loaded");
    buffer[bytesRead] = '\0';
    fclose(file);

    return config_from_json(&config, buffer);
}
