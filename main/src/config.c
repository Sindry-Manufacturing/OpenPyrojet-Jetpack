#include "config.h"
#include "esp_log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "cJSON.h"

const char* CONFIG_TAG = "config";
Config config;

const char* userConfigPath = "/config/user.json";
const char* defaultConfigPath = "/config/default.json";

static bool config_file_exists(const char* filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

esp_err_t config_read_nozzle_pins(Config* config, const cJSON* jsonArray) {
    if (!cJSON_IsArray(jsonArray)) {
        ESP_LOGE(CONFIG_TAG, "nozzlePins is not an array");
        return ESP_FAIL;
    }
    config->nozzleCount = cJSON_GetArraySize(jsonArray);
    int nozzleIndex = 0;
    cJSON* jsonPin = NULL;
    cJSON_ArrayForEach(jsonPin, jsonArray)
    {
        int pinNumber = jsonPin->valueint;
        config->nozzlePins[nozzleIndex] = pinNumber;
        ESP_LOGI(CONFIG_TAG, "nozzle %d: pin %d", nozzleIndex, pinNumber);
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
        ESP_LOGE(CONFIG_TAG, "wifi config not found");
        return ESP_FAIL;
    }
    const cJSON* ssidJson = cJSON_GetObjectItemCaseSensitive(wifiJson, "ssid");
    if (cJSON_IsNull(ssidJson) || !cJSON_IsString(ssidJson)) {
        ESP_LOGE(CONFIG_TAG, "wifi ssid is not a string");
        return ESP_FAIL;
    }

    const cJSON* passwordJson = cJSON_GetObjectItemCaseSensitive(wifiJson, "password");
    if (cJSON_IsNull(passwordJson) || !cJSON_IsString(passwordJson)) {
        ESP_LOGE(CONFIG_TAG, "wifi password is not a string");
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
    ESP_LOGI(CONFIG_TAG, "heatingDuration = %d µs", config->heatingDuration);

    config->triggerDelay = cJSON_GetObjectItemCaseSensitive(root, "triggerDelay")->valueint;
    ESP_LOGI(CONFIG_TAG, "triggerDelay = %d µs", config->triggerDelay);

    const cJSON* nozzlePinsJson = cJSON_GetObjectItemCaseSensitive(root, "nozzlePins");
    if (config_read_nozzle_pins(config, nozzlePinsJson) != ESP_OK) {
        return ESP_FAIL;
    }
    ESP_LOGI(CONFIG_TAG, "found %d nozzle pins", config->nozzleCount);

    cJSON_Delete(root);
    return ESP_OK;
}

bool config_ensure_user_copy() {
    if (config_file_exists(userConfigPath)) { // file exists
        ESP_LOGI(CONFIG_TAG, "using existing %s", userConfigPath);
        return true;
    }

    if (config_file_exists(defaultConfigPath)) { // file exists
        ESP_LOGI(CONFIG_TAG, "%s exists", defaultConfigPath);
    } else {
        ESP_LOGI(CONFIG_TAG, "%s not found", defaultConfigPath);
    }

    FILE* sourceFile = fopen(defaultConfigPath, "r");
    if (sourceFile == NULL) {
        ESP_LOGE(CONFIG_TAG, "failed to open target file %s", defaultConfigPath);
        return false;
    }

    FILE* targetFile = fopen(userConfigPath, "w");
    if (targetFile == NULL) {
        ESP_LOGE(CONFIG_TAG, "failed to open target file %s", userConfigPath);
        fclose(sourceFile);
        return false;
    }

    int character;
    while ((character = fgetc(sourceFile)) != EOF) {
        fputc(character, targetFile);
    }

    ESP_LOGI(CONFIG_TAG, "copied %s into %s", defaultConfigPath, userConfigPath);

    fclose(sourceFile);
    fclose(targetFile);

    return true;
}

esp_err_t config_init() {
    config_ensure_user_copy();

    const char* userConfigPath = "/config/user.json";
    FILE* file = fopen(userConfigPath, "r");
    if (file == NULL) {
        ESP_LOGE(CONFIG_TAG, "failed to open file %s", userConfigPath);
        return false;
    }

    // Verify file size
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    if (fileSize > CONFIG_FILE_SIZE_MAX) {
        ESP_LOGE(CONFIG_TAG, "file too large: %ld bytes", fileSize);
        return ESP_FAIL;
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
