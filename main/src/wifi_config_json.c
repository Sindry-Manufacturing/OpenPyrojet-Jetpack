#include "wifi_config_json.h"
#include <esp_log.h>
#include <stdlib.h>
#include <string.h>

static const char* TAG = "wifi-config-json";

bool wifi_config_from_json(const cJSON* json, WifiConfig* wifiConfig) {
    wifiConfig->ssid[0] = 0;
    wifiConfig->password[0] = 0;

    const cJSON* wifiJson = cJSON_GetObjectItemCaseSensitive(json, "wifi");
    if (cJSON_IsNull(wifiJson)) {
        ESP_LOGE(TAG, "wifi config not found");
        return false;
    }
    const cJSON* ssidJson = cJSON_GetObjectItemCaseSensitive(wifiJson, "ssid");
    if (cJSON_IsNull(ssidJson) || !cJSON_IsString(ssidJson)) {
        ESP_LOGE(TAG, "wifi ssid is not a string");
        return false;
    }

    const cJSON* passwordJson = cJSON_GetObjectItemCaseSensitive(wifiJson, "password");
    if (cJSON_IsNull(passwordJson) || !cJSON_IsString(passwordJson)) {
        ESP_LOGE(TAG, "wifi password is not a string");
        return false;
    }

    const char* ssid = cJSON_GetStringValue(ssidJson);
    const char* password = cJSON_GetStringValue(passwordJson);

    strcpy(wifiConfig->ssid, ssid);
    strcpy(wifiConfig->password, password);

    return true;
}

void wifi_config_to_json(cJSON* json, WifiConfig* config) {
    cJSON_AddStringToObject(json, "ssid", config->ssid);
    cJSON_AddStringToObject(json, "password", "");
}
