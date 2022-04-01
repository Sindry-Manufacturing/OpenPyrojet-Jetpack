#include <nvs_flash.h>
#include <esp_netif.h>
#include <esp_event.h>
#include <esp_log.h>

#include "sdkconfig.h"
#include "wifi_connect.h"
#include "wifi_ap.h"
#include "config.h"
#include "jetpack_main.h"
#include "fs_init.h"
#include "websocket_server_start.h"
#include "rest_server.h"
#include "display.h"

static const char* TAG = "application";

esp_err_t nvs_flash_init_safely() {
    esp_err_t flashInitResult = nvs_flash_init();
    if (flashInitResult == ESP_ERR_NVS_NO_FREE_PAGES || flashInitResult == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        flashInitResult = nvs_flash_init();
    }
    return flashInitResult;
}

void app_main(void) {
    ESP_ERROR_CHECK(nvs_flash_init_safely());
    ESP_ERROR_CHECK(fs_init());
    ESP_ERROR_CHECK(jetpack_init()); // Config and IO

    if (display_config_is_usable(&config.display)) {
        display_init(&config.display);
        display_show_header();
    }

    esp_ip4_addr_t noIp = {
        .addr = 0
    };

    if (wifi_config_is_usable(&config.wifi)) {
        // Start wifi client and websocket server ("normal mode")
        ESP_LOGI(TAG, "normal mode");
        display_show_wifi_normal_mode(noIp);
        ESP_ERROR_CHECK(esp_netif_init());
        ESP_ERROR_CHECK(esp_event_loop_create_default());
        wifi_config(&(config.wifi));
        ESP_ERROR_CHECK(wifi_connect());
        ESP_ERROR_CHECK(websocket_server_start());
    } else {
        // Start wifi Access Point and REST server ("configuration mode")
        ESP_LOGI(TAG, "configuration mode");
        display_show_wifi_ap_mode(CONFIG_AP_WIFI_SSID, CONFIG_AP_WIFI_PASSWORD, noIp);
        wifi_ap_init();
        wifi_ap_start();
        ESP_ERROR_CHECK(rest_server_start(MOUNT_POINT_WWW));
    }
}
