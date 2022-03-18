#include "jetpack_rest.h"
#include "jetpack_main.h"
#include "cJSON.h"
#include "esp_log.h"

static esp_err_t jetpack_parameters_put_handler(httpd_req_t* request) {
    char* buffer = rest_read_buffer(request);
    if (buffer == NULL) {
        return ESP_FAIL;
    }
    cJSON* root = cJSON_Parse(buffer);
    jetpackParameters.heatingDuration = cJSON_GetObjectItem(root, "heatingDuration")->valueint;
    jetpackParameters.triggerDelay = cJSON_GetObjectItem(root, "triggerDelay")->valueint;
    ESP_LOGI(REST_TAG, "heatingDuration = %d, triggerDelay = %d", jetpackParameters.heatingDuration, jetpackParameters.triggerDelay);
    cJSON_Delete(root);
    httpd_resp_sendstr(request, "");
    return ESP_OK;
}

static esp_err_t jetpack_parameters_get_handler(httpd_req_t *req) {
    httpd_resp_set_type(req, "application/json");
    cJSON* root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "heatingDuration", jetpackParameters.heatingDuration);
    cJSON_AddNumberToObject(root, "triggerDelay", jetpackParameters.triggerDelay);
    const char* sys_info = cJSON_Print(root);
    httpd_resp_sendstr(req, sys_info);
    free((void*)sys_info);
    cJSON_Delete(root);
    return ESP_OK;
}
/* Return jetpack specs and params */
static esp_err_t jetpack_info_get_handler(httpd_req_t *req) {
    httpd_resp_set_type(req, "application/json");
    cJSON* root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "nozzleCount", jetpackSpecifications.nozzleCount);
    cJSON_AddNumberToObject(root, "heatingDuration", jetpackParameters.heatingDuration);
    cJSON_AddNumberToObject(root, "triggerDelay", jetpackParameters.triggerDelay);
    const char* sys_info = cJSON_Print(root);
    httpd_resp_sendstr(req, sys_info);
    free((void*)sys_info);
    cJSON_Delete(root);
    return ESP_OK;
}

/* Fires the nozzle with the current parameters */
static esp_err_t jetpack_fire_handler(httpd_req_t* request) {
    char* buffer = rest_read_buffer(request);
    if (buffer == NULL) {
        return ESP_FAIL;
    }

    cJSON* root = cJSON_Parse(buffer);
    int nozzleId = cJSON_GetObjectItem(root, "nozzle")->valueint;
    ESP_LOGI(REST_TAG, "Firing nozzle %d", nozzleId);
    cJSON_Delete(root);
    httpd_resp_sendstr(request, "");

    // TODO: Move to a different thread, so we don't have to wait for requests to finish sending
    jetpack_fire(nozzleId);

    return ESP_OK;
}

void jetpack_register_uri_handler(
    httpd_handle_t server,
    rest_server_context_t* rest_context
) {
    httpd_uri_t jetpack_info_get_uri = {
        .uri = "/api/v1/jetpack/info",
        .method = HTTP_GET,
        .handler = jetpack_info_get_handler,
        .user_ctx = rest_context
    };
    httpd_register_uri_handler(server, &jetpack_info_get_uri);

    httpd_uri_t jetpack_parameters_get_uri = {
        .uri = "/api/v1/jetpack/parameters",
        .method = HTTP_GET,
        .handler = jetpack_parameters_get_handler,
        .user_ctx = rest_context
    };
    httpd_register_uri_handler(server, &jetpack_parameters_get_uri);

    httpd_uri_t jetpack_parameters_put_uri = {
        .uri = "/api/v1/jetpack/parameters",
        .method = HTTP_PUT,
        .handler = jetpack_parameters_put_handler,
        .user_ctx = rest_context
    };
    httpd_register_uri_handler(server, &jetpack_parameters_put_uri);

    httpd_uri_t jetpack_fire_uri = {
        .uri = "/api/v1/jetpack/fire",
        .method = HTTP_POST,
        .handler = jetpack_fire_handler,
        .user_ctx = rest_context
    };
    httpd_register_uri_handler(server, &jetpack_fire_uri);
}
