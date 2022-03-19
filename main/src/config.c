#include "config.h"
#include "esp_log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

const char* CONFIG_TAG = "config";
Config config;

esp_err_t config_from_json(Config* config, const char* jsonBuffer) {
    cJSON* root = cJSON_Parse(jsonBuffer);

    config->heatingDuration = cJSON_GetObjectItemCaseSensitive(root, "heatingDuration")->valueint;
    ESP_LOGI(CONFIG_TAG, "heatingDuration = %d ms", config->heatingDuration);

    config->triggerDelay = cJSON_GetObjectItemCaseSensitive(root, "triggerDelay")->valueint;
    ESP_LOGI(CONFIG_TAG, "triggerDelay = %d ms", config->triggerDelay);

    const cJSON* nozzlePinsJson = cJSON_GetObjectItemCaseSensitive(root, "nozzlePins");
    if (!cJSON_IsArray(nozzlePinsJson)) {
        ESP_LOGE(CONFIG_TAG, "nozzlePins is not an array");
        return ESP_FAIL;
    }
    config->nozzleCount = cJSON_GetArraySize(nozzlePinsJson);
    int nozzleIndex = 0;
    cJSON* nozzlePinJson = NULL;
    cJSON_ArrayForEach(nozzlePinJson, nozzlePinsJson) {
        int pinNumber = nozzlePinJson->valueint;
        config->nozzlePins[nozzleIndex] = pinNumber;
        ESP_LOGI(CONFIG_TAG, "nozzle %d: pin %d", nozzleIndex, pinNumber);
        nozzleIndex++;
    }
    for (; nozzleIndex < (config->nozzleCount - 1); nozzleIndex++) {
        config->nozzlePins[nozzleIndex] = 0; // set the undefined pins to 0
    }

    cJSON_Delete(root);
    return ESP_OK;
}

esp_err_t config_init() {
    const char* filePath = "/config/default.json";
    FILE* file = fopen(filePath, "r");
    if (file == NULL) {
        ESP_LOGE(CONFIG_TAG, "failed to open file %s", filePath);
        return ESP_FAIL;
    }

    // Verify file size
    fseek(file, 0 , SEEK_END);
    long fileSize = ftell(file);
    if (fileSize > CONFIG_FILE_SIZE_MAX) {
        ESP_LOGE(CONFIG_TAG, "file too large: %ld bytes", fileSize);
    }
    rewind(file);

    char buffer[fileSize + 1]; // needs space for null terminator
    size_t bytesRead = fread(buffer, 1, fileSize, file);
    if (ferror(file) != 0) {
        ESP_LOGE(CONFIG_TAG, "read error");
        fclose(file);
        return ESP_FAIL;
    }

    ESP_LOGI(CONFIG_TAG, "loaded");
    buffer[bytesRead] = '\0';
    fclose(file);

    return config_from_json(&config, buffer);
}
