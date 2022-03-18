/**
 * Shared code for all REST API implementations.
 */
#ifndef OPENPYROJET_REST_BASE_H
#define OPENPYROJET_REST_BASE_H

#include "esp_http_server.h"
#include "esp_vfs.h"

#define SCRATCH_BUFSIZE (10240)

typedef struct rest_server_context {
    char base_path[ESP_VFS_PATH_MAX + 1];
    char scratch[SCRATCH_BUFSIZE];
} rest_server_context_t;

#define REST_TAG "rest"

#define REST_CHECK(a, str, goto_tag, ...) \
    do \
    { \
        if (!(a)) \
        { \
            ESP_LOGE(REST_TAG, "%s(%d): " str, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
            goto goto_tag; \
        } \
    } while (0)

char* rest_read_buffer(httpd_req_t* request);

#endif //OPENPYROJET_REST_BASE_H
