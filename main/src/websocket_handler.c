#include "websocket_handler.h"
#include <esp_log.h>

#include "message_json.h"
#include "config_json.h"

static const char* TAG = "websocket-handler";

static esp_err_t websocket_handler(httpd_req_t*);

const httpd_uri_t websocketHandlerUri = {
    .uri        = "/",
    .method     = HTTP_GET,
    .handler    = websocket_handler,
    .user_ctx   = NULL,
    .is_websocket = true
};

static esp_err_t handle_get_config(httpd_req_t* request) {
    cJSON* json = config_to_json(&config);
    char* jsonText = cJSON_PrintUnformatted(json);
    httpd_ws_frame_t frame;
    frame.payload = (uint8_t*)jsonText;
    frame.type = HTTPD_WS_TYPE_TEXT;
    frame.len = strlen((const char*)jsonText);
    esp_err_t result = httpd_ws_send_frame(request, &frame);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "sending response failed (%d)", result);
    }
    cJSON_Delete(json);
    return result;
}

static esp_err_t websocket_handler(httpd_req_t* request) {
    uint8_t buf[128] = {0};
    httpd_ws_frame_t frame;
    memset(&frame, 0, sizeof(httpd_ws_frame_t));
    frame.payload = buf;
    frame.type = HTTPD_WS_TYPE_TEXT;
    esp_err_t result = httpd_ws_recv_frame(request, &frame, 128);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "httpd_ws_recv_frame failed with %d", result);
        return result;
    }
    ESP_LOGI(TAG, "Received[%d]: %s", frame.type, frame.payload);

    if (frame.type == HTTPD_WS_TYPE_TEXT) {
        Message message;
        cJSON* messageJson = message_from_json(&message, (const char*)frame.payload);
        if (messageJson == NULL) {
            ESP_LOGW(TAG, "no message found");
        }
        if (messageJson != NULL) {
            ESP_LOGI(TAG, "received Message with type %d", message.type);
            switch (message.type) {
                case GET_CONFIG:
                    result = handle_get_config(request);
                    break;
                case PUT_CONFIG:
                    break;
                case FIRE_NOZZLE:
                    break;
                default:
                    break;
            }
            cJSON_Delete(messageJson);
        }
    } else {
        ESP_LOGI(TAG, "received other");
    }
    return result;
}
