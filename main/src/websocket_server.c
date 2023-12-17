#include "websocket_server.h"

#include <esp_log.h>

#include "config.h"

/*
 * Structure holding server handle and internal socket fd in order to use out of request send
 */
struct async_resp_arg {
    httpd_handle_t hd;
    int fd;
};

static httpd_uri_t* configuredUri = NULL;

static void websocket_async_send(void*);

static const char* TAG = "websocket";

void websocket_config(httpd_uri_t* uri) {
    configuredUri = uri;
}

/*
 * async send function, which we put into the httpd work queue
 */
static void websocket_async_send(void* arg) {
    struct async_resp_arg* resp_arg = (struct async_resp_arg*)arg;
    static const char* data = "Async data";
    httpd_handle_t hd = resp_arg->hd;
    int fd = resp_arg->fd;
    httpd_ws_frame_t ws_pkt;
    memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
    ws_pkt.payload = (uint8_t*) data;
    ws_pkt.len = strlen(data);
    ws_pkt.type = HTTPD_WS_TYPE_TEXT;

    httpd_ws_send_frame_async(hd, fd, &ws_pkt);
    free(resp_arg);
}

esp_err_t websocket_trigger_async_send(httpd_handle_t handle, httpd_req_t* request) {
    struct async_resp_arg* response = malloc(sizeof(struct async_resp_arg));
    response->hd = request->handle;
    response->fd = httpd_req_to_sockfd(request);
    return httpd_queue_work(handle, websocket_async_send, response);
}


httpd_handle_t websocket_start_server() {
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        // Registering the ws handler
        ESP_LOGI(TAG, "Registering URI handlers");
        if (configuredUri == NULL)  {
            ESP_LOGE(TAG, "configuredUri is NULL");
        }
        httpd_register_uri_handler(server, configuredUri);
        return server;
    }

    ESP_LOGI(TAG, "Error starting server!");
    return NULL;
}

static void websocket_stop_server(httpd_handle_t server) {
    // Stop the httpd server
    httpd_stop(server);
}

void websocket_disconnect_handler(
    void* arg,
    esp_event_base_t eventBase,
    int32_t eventId,
    void* eventData
) {
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server) {
        ESP_LOGI(TAG, "Stopping webserver");
        websocket_stop_server(*server);
        *server = NULL;
    }
}

void websocket_connect_handler(
    void* arg,
    esp_event_base_t eventBase,
    int32_t eventId,
    void* eventData
) {
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server == NULL) {
        ESP_LOGI(TAG, "Starting webserver");
        *server = websocket_start_server();
    }
}
