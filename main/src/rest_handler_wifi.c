#include "rest_handler_wifi.h"

#include <cJSON.h>
#include <esp_log.h>

#include "app_state.h"

static esp_err_t wifi_put_handler(httpd_req_t* request) {
    const char* buffer = rest_read_buffer(request);
    if (buffer == NULL) {
        return ESP_FAIL;
    }
    cJSON* root = cJSON_Parse(buffer);
    const char* wifiSsid = cJSON_GetObjectItem(root, "ssid")->valuestring;
    unsigned long wifiSsidLength = strlen(wifiSsid);
    const char* wifiPassword = cJSON_GetObjectItem(root, "password")->valuestring;
    unsigned long wifiPasswordLength = strlen(wifiPassword);

    bool isValid = wifiSsidLength < sizeof(appState.config.wifi.ssid) &&
        wifiPasswordLength < sizeof(appState.config.wifi.password);

    if (isValid) {
        strcpy((char*)appState.config.wifi.ssid, wifiSsid);
        strcpy((char*)appState.config.wifi.password, wifiPassword);
        config_save(&appState.config);
        ESP_LOGI(REST_TAG, "wifi config: SSID %s, password (%d characters)", wifiSsid, strlen(wifiPassword));
    } else {
        ESP_LOGE(REST_TAG, "wifi SSID or password too long");
    }

    cJSON_Delete(root);
    httpd_resp_sendstr(request, "");

    return ESP_OK;
}

void register_wifi_uri_handler(
    httpd_handle_t server,
    rest_server_context_t* rest_context
) {
    httpd_uri_t wifi_put_uri = {
        .uri = "/api/wifi",
        .method = HTTP_PUT,
        .handler = wifi_put_handler,
        .user_ctx = rest_context
    };
    httpd_register_uri_handler(server, &wifi_put_uri);
}
