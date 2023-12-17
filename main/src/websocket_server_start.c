/**
 * This code is based on:
 *   $IDF_PATH/examples/protocols/http_server/ws_echo_server
 */
#include "websocket_server_start.h"

#include <esp_event.h>
#include <esp_wifi.h> // for WIFI_EVENT
#include <esp_netif.h> // for IP_EVENT

#include "websocket_server.h"
#include "websocket_handler.h"

esp_err_t websocket_server_start() {
    static httpd_handle_t server = NULL;
    websocket_config(&websocketHandlerUri);
    // Start/stop server when wifi connects or disconnects
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &websocket_connect_handler, &server));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &websocket_disconnect_handler, &server));
    server = websocket_start_server();
    return ESP_OK;
}
