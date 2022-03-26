#ifndef OPENPYROJET_WEBSOCKET_SERVER_H
#define OPENPYROJET_WEBSOCKET_SERVER_H

#include <esp_event.h>
#include <esp_http_server.h>

void websocket_config(httpd_uri_t* uri);

httpd_handle_t websocket_start_server();

void websocket_disconnect_handler(
    void* arg,
    esp_event_base_t eventBase,
    int32_t eventId,
    void* eventData
);

void websocket_connect_handler(
    void* arg,
    esp_event_base_t eventBase,
    int32_t eventId,
    void* eventData
);

esp_err_t websocket_trigger_async_send(httpd_handle_t handle, httpd_req_t* request);

#endif //OPENPYROJET_WEBSOCKET_SERVER_H
