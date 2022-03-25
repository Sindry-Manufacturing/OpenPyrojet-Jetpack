#ifndef OPENPYROJET_WEBSOCKET_SERVER_START_H
#define OPENPYROJET_WEBSOCKET_SERVER_START_H

#include <esp_err.h>

/**
 * The main entry point for starting the websocket server.
 * This is generally called from main.c after WiFi is configured.
 */
esp_err_t websocket_server_start();

#endif //OPENPYROJET_WEBSOCKET_SERVER_START_H
