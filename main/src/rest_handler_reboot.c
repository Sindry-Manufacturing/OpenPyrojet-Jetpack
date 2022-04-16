#include "rest_handler_reboot.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>

static const char* TAG = "rest-handler-reboot";

static void reboot_task(void* parameter) {
    ESP_LOGI(TAG, "rebooting");
    // Delay for the REST request finish
    vTaskDelay(500L / portTICK_PERIOD_MS);
    esp_restart();
    vTaskDelete(NULL); // never gets executed
}

static esp_err_t reboot_post_handler(httpd_req_t* request) {
    httpd_resp_sendstr(request, "");
    ESP_LOGI(TAG, "starting reboot task");
    static uint8_t taskParameter;
    TaskHandle_t taskHandle = NULL;
    xTaskCreate(
        reboot_task,
        "reboot",
        4,
        &taskParameter,
        tskIDLE_PRIORITY,
        &taskHandle
    );
    configASSERT(taskHandle);
    return ESP_OK;
}

void register_reboot_uri_handler(
    httpd_handle_t server,
    rest_server_context_t* restContext
) {
    httpd_uri_t rebootPostUri = {
        .uri = "/api/reboot",
        .method = HTTP_POST,
        .handler = reboot_post_handler,
        .user_ctx = restContext
    };
    httpd_register_uri_handler(server, &rebootPostUri);
}
