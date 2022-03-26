#include <string.h>
#include "esp_system.h"
#include "esp_log.h"
#include "cJSON.h"
#include "rest_base.h"
#include "rest_wildcard.h"
#include "rest_system_info.h"
#include "rest_wifi.h"

esp_err_t rest_server_start(const char* basePath) {
    REST_CHECK(basePath, "wrong base path", err);
    rest_server_context_t* rest_context = calloc(1, sizeof(rest_server_context_t));
    REST_CHECK(rest_context, "No memory for rest context", err);
    strlcpy(rest_context->base_path, basePath, sizeof(rest_context->base_path));

    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.uri_match_fn = httpd_uri_match_wildcard;

    ESP_LOGI(REST_TAG, "starting");
    REST_CHECK(httpd_start(&server, &config) == ESP_OK, "Start server failed", err_start);

    system_info_register_uri_handler(server, rest_context);
    register_wifi_uri_handler(server, rest_context);
    register_wildcard_uri_handler(server, rest_context);

    ESP_LOGI(REST_TAG, "ready");
    return ESP_OK;
err_start:
    free(rest_context);
err:
    return ESP_FAIL;
}
