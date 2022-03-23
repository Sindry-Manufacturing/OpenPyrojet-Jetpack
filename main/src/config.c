#include "config.h"
#include "config_json.h"
#include <esp_log.h>

#include "cJSON.h"

#include "file.h"

static const char* TAG = "config";
static const char* userConfigPath = "/config/user.json";
static const char* defaultConfigPath = "/config/default.json";

Config config;


bool config_ensure_user_copy() {
    if (file_exists(userConfigPath)) { // file exists
        ESP_LOGI(TAG, "using existing %s", userConfigPath);
        return true;
    } else {
        return file_copy(defaultConfigPath, userConfigPath);
    }
}

bool config_init() {
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
