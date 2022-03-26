#include <driver/gpio.h>
#include <nvs_flash.h>
#include <esp_system.h>
#include <esp_netif.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_http_server.h>
#include "sdkconfig.h"

#include "wifi_connect.h"
#include "wifi_ap.h"
#include "config.h"
#include "jetpack_main.h"
#include "fs_init.h"
#include "websocket_server_start.h"
#include "rest_server.h"

static const char* TAG = "application";

void app_main(void) {
    esp_err_t flashInitResult = nvs_flash_init();
    if (flashInitResult == ESP_ERR_NVS_NO_FREE_PAGES || flashInitResult == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        flashInitResult = nvs_flash_init();
    }
    ESP_ERROR_CHECK(flashInitResult);

    ESP_ERROR_CHECK(fs_init());
    ESP_ERROR_CHECK(jetpack_init()); // Config and IO

    if (strlen(config.wifi.ssid) > 0) {
        ESP_ERROR_CHECK(esp_netif_init());
        ESP_ERROR_CHECK(esp_event_loop_create_default());

        ESP_LOGI(TAG, "normal mode");

        wifi_config(&(config.wifi));

        ESP_ERROR_CHECK(wifi_connect());
        ESP_ERROR_CHECK(websocket_server_start());
    } else {
        ESP_LOGI(TAG, "configuration mode");
        wifi_ap_init();
        wifi_ap();
        ESP_ERROR_CHECK(rest_server_start(MOUNT_POINT_WWW));
    }
}
