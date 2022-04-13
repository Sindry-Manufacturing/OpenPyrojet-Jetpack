#include "wifi_config_json.h"

#include <esp_log.h>
#include <string.h>

static const char* TAG = "wifi-config-json";

bool wifi_config_from_json(const cJSON* json, WifiConfig* wifiConfig) {
    memset(wifiConfig->ssid, 0, sizeof(wifiConfig->ssid));
    memset(wifiConfig->password, 0, sizeof(wifiConfig->password));

    const cJSON* ssidJson = cJSON_GetObjectItemCaseSensitive(json, "ssid");
    if (cJSON_IsNull(ssidJson) || !cJSON_IsString(ssidJson)) {
        ESP_LOGE(TAG, "wifi ssid is not a string");
        return false;
    }

    const cJSON* passwordJson = cJSON_GetObjectItemCaseSensitive(json, "password");
    if (cJSON_IsNull(passwordJson) || !cJSON_IsString(passwordJson)) {
        ESP_LOGE(TAG, "wifi password is not a string");
        return false;
    }

    const char* ssid = cJSON_GetStringValue(ssidJson);
    const char* password = cJSON_GetStringValue(passwordJson);

    size_t ssidLength = strlen(ssid);
    size_t passwordLength = strlen(password);
    bool isValid = ssidLength < sizeof(wifiConfig->ssid) &&
        passwordLength < sizeof(wifiConfig->password);

    if (isValid) {
        strcpy(wifiConfig->ssid, ssid);
        strcpy(wifiConfig->password, password);
        return true;
    } else {
        ESP_LOGE(TAG, "wifi password and/or ssid are too long");
        return false;
    }
}

static void wifi_config_to_json_internal(cJSON* json, const WifiConfig* config, bool removeRiskItems) {
    cJSON_AddStringToObject(json, "ssid", config->ssid);
    if (!removeRiskItems) {
        cJSON_AddStringToObject(json, "password", config->password);
    } else {
        cJSON_AddNullToObject(json, "password");
    }
}

void wifi_config_to_json(cJSON* root, const WifiConfig* config) {
    wifi_config_to_json_internal(root, config, false);
}

void wifi_config_to_json_safe(cJSON* root, const WifiConfig* config) {
    wifi_config_to_json_internal(root, config, true);
}
