#include "rest_handler_config.h"

#include <cJSON.h>
#include <esp_log.h>

#include "app_state.h"
#include "wifi_config_json.h"

static esp_err_t config_put_handler(httpd_req_t* request) {
    const char* buffer = rest_read_buffer(request);
    if (buffer == NULL) {
        return ESP_FAIL;
    }

    cJSON* rootJson = cJSON_Parse(buffer);
    const cJSON* wifiJson = cJSON_GetObjectItemCaseSensitive(rootJson, "wifi");
    esp_err_t result = ESP_FAIL;
    if (cJSON_IsObject(wifiJson)) {
        if (wifi_config_from_json(wifiJson, &appState.config.wifi)) {
            config_save(&appState.config);
            ESP_LOGI(REST_TAG, "wifi config saved");
            result = ESP_OK;
        } else {
            ESP_LOGE(REST_TAG, "wifi config invalid (invalid type or invalid length))");
        }
    } else {
        ESP_LOGE(REST_TAG, "wifi object not found in json");
    }

    cJSON_Delete(rootJson);
    httpd_resp_sendstr(request, "");

    return result;
}

void register_config_uri_handler(
    httpd_handle_t server,
    rest_server_context_t* restContext
) {
    httpd_uri_t configPutUri = {
        .uri = "/api/config",
        .method = HTTP_PUT,
        .handler = config_put_handler,
        .user_ctx = restContext
    };
    httpd_register_uri_handler(server, &configPutUri);
}
