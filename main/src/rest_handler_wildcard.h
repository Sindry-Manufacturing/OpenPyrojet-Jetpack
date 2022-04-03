/**
 * REST URI handlers for catch-all file serving (static files in ESP32 partition)
 */
#ifndef OPENPYROJET_REST_HANDLER_WILDCARD_H
#define OPENPYROJET_REST_HANDLER_WILDCARD_H

#include "esp_http_server.h"
#include "rest_base.h"

void register_wildcard_uri_handler(
    httpd_handle_t,
    rest_server_context_t*
);

#endif //OPENPYROJET_REST_HANDLER_WILDCARD_H
