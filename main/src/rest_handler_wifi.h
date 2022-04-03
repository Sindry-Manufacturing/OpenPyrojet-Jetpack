#ifndef OPENPYROJET_REST_HANDLER_WIFI_H
#define OPENPYROJET_REST_HANDLER_WIFI_H

#include <esp_http_server.h>

#include "rest_utils.h"

void register_wifi_uri_handler(
    httpd_handle_t server,
    rest_server_context_t* rest_context
);

#endif //OPENPYROJET_REST_HANDLER_WIFI_H