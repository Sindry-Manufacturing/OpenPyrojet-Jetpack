#ifndef OPENPYROJET_JETPACK_REST_H
#define OPENPYROJET_JETPACK_REST_H

#include "esp_http_server.h"
#include "rest_base.h"

void jetpack_register_uri_handler(
    httpd_handle_t,
    rest_server_context_t*
);

#endif //OPENPYROJET_JETPACK_REST_H
