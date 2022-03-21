#include <driver/gpio.h>
#include <nvs_flash.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <esp_netif.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_http_server.h>
#include <mdns.h>

#include "sdkconfig.h"

#include "wifi_connect.h"
#include "config.h"
#include "jetpack_main.h"
#include "rest_server.h"
#include "fs_init.h"

/*
 * Structure holding server handle and internal socket fd in order to use out of request send
 */
struct async_resp_arg {
    httpd_handle_t hd;
    int fd;
};

static esp_err_t websocket_handler(httpd_req_t*);
static void ws_async_send(void*);

static const httpd_uri_t server_uri = {
    .uri        = "/",
    .method     = HTTP_GET,
    .handler    = websocket_handler,
    .user_ctx   = NULL,
    .is_websocket = true
};

static const char* TAG = "main";

/*
 * async send function, which we put into the httpd work queue
 */
static void ws_async_send(void* arg) {
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

static esp_err_t trigger_async_send(httpd_handle_t handle, httpd_req_t* request) {
    struct async_resp_arg* response = malloc(sizeof(struct async_resp_arg));
    response->hd = request->handle;
    response->fd = httpd_req_to_sockfd(request);
    return httpd_queue_work(handle, ws_async_send, response);
}

/*
 * This handler echos back the received ws data
 * and triggers an async send if certain message received
 */
static esp_err_t websocket_handler(httpd_req_t* request) {
    uint8_t buf[128] = {0};
    httpd_ws_frame_t frame;
    memset(&frame, 0, sizeof(httpd_ws_frame_t));
    frame.payload = buf;
    frame.type = HTTPD_WS_TYPE_TEXT;
    esp_err_t ret = httpd_ws_recv_frame(request, &frame, 128);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "httpd_ws_recv_frame failed with %d", ret);
        return ret;
    }
    ESP_LOGI(TAG, "Received[%d]: %s", frame.type, frame.payload);
    if (frame.type == HTTPD_WS_TYPE_TEXT &&
        strcmp((char*) frame.payload, "Trigger async") == 0) {
        return trigger_async_send(request->handle, request);
    }

    ret = httpd_ws_send_frame(request, &frame);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "httpd_ws_send_frame failed with %d", ret);
    }
    return ret;
}

static httpd_handle_t start_webserver() {
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        // Registering the ws handler
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &server_uri);
        return server;
    }

    ESP_LOGI(TAG, "Error starting server!");
    return NULL;
}

static void stop_webserver(httpd_handle_t server) {
    // Stop the httpd server
    httpd_stop(server);
}

static void disconnect_handler(
    void* arg,
    esp_event_base_t event_base,
    int32_t event_id,
    void* event_data
) {
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server) {
        ESP_LOGI(TAG, "Stopping webserver");
        stop_webserver(*server);
        *server = NULL;
    }
}

static void connect_handler(
    void* arg,
    esp_event_base_t event_base,
    int32_t event_id,
    void* event_data
) {
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server == NULL) {
        ESP_LOGI(TAG, "Starting webserver");
        *server = start_webserver();
    }
}

void app_main(void) {
    static httpd_handle_t server = NULL;

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(fs_init());

    ESP_ERROR_CHECK(jetpack_init());

    wifi_config(&(config.wifi));

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(wifi_connect());

    // Start/stop server when wifi connects or disconnects
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &connect_handler, &server));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &disconnect_handler, &server));

    server = start_webserver();
}
