#ifndef OPENPYROJET_REST_SERVER_H
#define OPENPYROJET_REST_SERVER_H

#include "esp_http_server.h"

esp_err_t rest_server_start(const char* basePath);

#endif //OPENPYROJET_REST_SERVER_H
