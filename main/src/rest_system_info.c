#include "rest_system_info.h"
#include "cJSON.h"
#include "esp_log.h"

static esp_err_t system_info_get_handler(httpd_req_t* request)
{
    httpd_resp_set_type(request, "application/json");
    cJSON* root = cJSON_CreateObject();
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    cJSON_AddStringToObject(root, "version", IDF_VER);
    cJSON_AddNumberToObject(root, "cores", chip_info.cores);
    const char* systemInfo = cJSON_Print(root);
    httpd_resp_sendstr(request, systemInfo);
    free((void*)systemInfo);
    cJSON_Delete(root);
    return ESP_OK;
}

void system_info_register_uri_handler(
    httpd_handle_t server,
    rest_server_context_t* rest_context
) {
    httpd_uri_t system_info_get_uri = {
        .uri = "/api/v1/system/info",
        .method = HTTP_GET,
        .handler = system_info_get_handler,
        .user_ctx = rest_context
    };
    httpd_register_uri_handler(server, &system_info_get_uri);
}
