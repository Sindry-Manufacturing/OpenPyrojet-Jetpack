#include "fs_init.h"
#include "esp_spiffs.h"
#include "esp_log.h"
#include "esp_event.h"

static const char* TAG = "filesystem";

esp_err_t fs_init_internal(esp_vfs_spiffs_conf_t* conf) {
    esp_err_t ret = esp_vfs_spiffs_register(conf);
    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem %s", conf->base_path);
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition %s", conf->base_path);
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS %s (%s)", conf->base_path, esp_err_to_name(ret));
        }
        return ESP_FAIL;
    }

    size_t total = -1, used = 0;
    ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information for %s (%s)", conf->base_path, esp_err_to_name(ret));
    } else {
        ESP_LOGI(TAG, "Partition size for %s: total: %d, used: %d", conf->base_path, total, used);
    }
    return ESP_OK;
}

esp_err_t fs_init() {
    esp_vfs_spiffs_conf_t wwwSpiffs = {
        .base_path = MOUNT_POINT_WWW,
        .partition_label = NULL,
        .max_files = 4,
        .format_if_mount_failed = false
    };

    if (fs_init_internal(&wwwSpiffs) != ESP_OK) {
        return ESP_FAIL;
    }

    esp_vfs_spiffs_conf_t configSpiffs = {
        .base_path = MOUNT_POINT_CONFIG,
        .partition_label = "config",
        .max_files = 2,
        .format_if_mount_failed = false
    };

    if (fs_init_internal(&configSpiffs) != ESP_OK) {
        return ESP_FAIL;
    }

    return ESP_OK;
}
