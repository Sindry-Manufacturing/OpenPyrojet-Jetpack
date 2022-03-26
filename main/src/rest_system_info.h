#ifndef OPENPYROJET_REST_SYSTEM_INFO_H
#define OPENPYROJET_REST_SYSTEM_INFO_H

#include <esp_http_server.h>
#include "rest_base.h"

void system_info_register_uri_handler(httpd_handle_t, rest_server_context_t*);

#endif //OPENPYROJET_REST_SYSTEM_INFO_H
