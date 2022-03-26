#include "websocket_handler.h"
#include <esp_log.h>

#include "message_json.h"
#include "config_json.h"
#include "jetpack_main.h"

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
    cJSON* dataJson = config_to_json_safe(&config);
    cJSON* messageJson = message_json_object("config", dataJson);
    char* messageJsonText = cJSON_PrintUnformatted(messageJson);

    httpd_ws_frame_t frame;
    frame.payload = (uint8_t*)messageJsonText;
    frame.type = HTTPD_WS_TYPE_TEXT;
    frame.len = strlen((const char*)messageJsonText);
    esp_err_t result = httpd_ws_send_frame(request, &frame);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "sending response failed (%d)", result);
    }

    // We only need to delete the root cJSON, because the data cJSON gets deleted with it
    cJSON_Delete(messageJson);

    return result;
}

static esp_err_t handle_put_config(const Message* message) {
    if (!config_from_json(&config, message->data)) {
        ESP_LOGW(TAG, "failed to parse incoming Message data as Config");
        return ESP_FAIL;
    }

    if (!config_save(&config)) {
        ESP_LOGE(TAG, "failed to save config");
        return ESP_FAIL;
    }

    return ESP_OK;
}

static esp_err_t handle_fire_nozzle(const Message* message) {
    if (!cJSON_IsNumber(message->data)) {
        ESP_LOGE(TAG, "nozzle id is not a number");
        return ESP_FAIL;
    }

    long nozzleId = cJSON_GetNumberValue(message->data);
    jetpack_fire((uint8)nozzleId);

    return ESP_OK;
}

static esp_err_t websocket_handler(httpd_req_t* request) {
    uint8_t payload[128] = {0};
    httpd_ws_frame_t frame;
    memset(&frame, 0, sizeof(httpd_ws_frame_t));
    frame.payload = payload;
    frame.type = HTTPD_WS_TYPE_TEXT;
    esp_err_t result = httpd_ws_recv_frame(request, &frame, 128);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "httpd_ws_recv_frame failed with %d", result);
        return result;
    }
    ESP_LOGI(TAG, "Received[%d]: %s", frame.type, frame.payload);

    if (frame.type == HTTPD_WS_TYPE_TEXT) {
        Message message;
        cJSON* messageJson = message_from_json(&message, (const char*) frame.payload);
        if (messageJson == NULL) {
            ESP_LOGW(TAG, "no message found");
        }
        if (messageJson != NULL) {
            ESP_LOGI(TAG, "received Message with type %s", message.type);
            if (strcmp(message.type, INCOMING_MESSAGE_TYPE_GET_CONFIG) == 0) {
                result = handle_get_config(request);
            } else if (strcmp(message.type, INCOMING_MESSAGE_TYPE_PUT_CONFIG) == 0) {
                result = handle_put_config(&message);
            } else if (strcmp(message.type, INCOMING_MESSAGE_TYPE_FIRE_NOZZLE) == 0) {
                result = handle_fire_nozzle(&message);
            } else {
            }
            cJSON_Delete(messageJson);
        }
    } else {
        ESP_LOGI(TAG, "received other");
    }
    return result;
}