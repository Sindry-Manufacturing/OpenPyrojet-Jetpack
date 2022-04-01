/**
 * This code is based on:
 *   $IDF_PATH/examples/protocols/http_server/ws_echo_server
 */
#include "websocket_handler.h"

#include <esp_log.h>

#include "message_json.h"
#include "config_json.h"
#include "jetpack_main.h"

static const char* TAG = "websocket-handler";

static esp_err_t websocket_handler(httpd_req_t*);

httpd_uri_t websocketHandlerUri = {
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

static esp_err_t websocket_handle_text_frame(httpd_req_t* request, const httpd_ws_frame_t* frame) {
    Message message;
    cJSON* messageJson = message_from_json(&message, (const char*) frame->payload);
    if (messageJson == NULL) {
        ESP_LOGW(TAG, "No message found");
    }
    esp_err_t result = ESP_FAIL;
    if (messageJson != NULL) {
        ESP_LOGI(TAG, "Received Message with type %s", message.type);
        if (strcmp(message.type, INCOMING_MESSAGE_TYPE_GET_CONFIG) == 0) {
            result = handle_get_config(request);
        } else if (strcmp(message.type, INCOMING_MESSAGE_TYPE_PUT_CONFIG) == 0) {
            result = handle_put_config(&message);
        } else if (strcmp(message.type, INCOMING_MESSAGE_TYPE_FIRE_NOZZLE) == 0) {
            result = handle_fire_nozzle(&message);
        } else {
            result = ESP_ERR_INVALID_RESPONSE;
        }
        cJSON_Delete(messageJson);
    } else {
        result = ESP_ERR_INVALID_RESPONSE;
    }

    return result;
}

static esp_err_t websocket_handler(httpd_req_t* request) {
    if (request->method == HTTP_GET) {
        ESP_LOGI(TAG, "Handshake done, the new connection was opened");
        return ESP_OK;
    }

    uint8_t* buffer = NULL;
    httpd_ws_frame_t frame;
    memset(&frame, 0, sizeof(httpd_ws_frame_t));
    frame.type = HTTPD_WS_TYPE_TEXT;
    // max_len 0 to get frame length
    esp_err_t receiveResult = httpd_ws_recv_frame(request, &frame, 0);
    if (receiveResult != ESP_OK) {
        ESP_LOGE(TAG, "httpd_ws_recv_frame failed to get frame (error %d)", receiveResult);
        return receiveResult;
    }

    ESP_LOGI(TAG, "Receiving frame (length %d, type %d)", frame.len, frame.type);

    if (frame.len > 0) {
        // frame.len + 1 is for NULL termination because we expect a string
        buffer = calloc(1, frame.len + 1);
        if (buffer == NULL) {
            ESP_LOGE(TAG, "Failed to allocate buffer");
            return ESP_ERR_NO_MEM;
        }
        frame.payload = buffer;
        // TODO: Re-write so we can handle larger frames. Large frames will currently fail to allocate.
        // max_len = frame.len to get the entire frame payload
        receiveResult = httpd_ws_recv_frame(request, &frame, frame.len);
        if (receiveResult != ESP_OK) {
            ESP_LOGE(TAG, "httpd_ws_recv_frame failed (error %d)", receiveResult);
            free(buffer);
            return receiveResult;
        }
        ESP_LOGI(TAG, "Received: %s", frame.payload);
    }

    esp_err_t handleResult;
    if (frame.type == HTTPD_WS_TYPE_TEXT) {
        handleResult = websocket_handle_text_frame(request, &frame);
    } else {
        handleResult = ESP_FAIL;
    }

    if (handleResult != ESP_OK) {
        ESP_LOGE(TAG, "Handling failed (error %d)", handleResult);
    }

    if (buffer != NULL) {
        free(buffer);
    }

    return handleResult;
}

