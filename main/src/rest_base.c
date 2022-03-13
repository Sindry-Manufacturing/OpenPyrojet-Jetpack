#include "rest_base.h"

char* rest_read_buffer(httpd_req_t* request) {
    int total_len = request->content_len;
    int cur_len = 0;
    char* buffer = ((rest_server_context_t *)(request->user_ctx))->scratch;
    int received = 0;
    if (total_len >= SCRATCH_BUFSIZE) {
        /* Respond with 500 Internal Server Error */
        httpd_resp_send_err(request, HTTPD_500_INTERNAL_SERVER_ERROR, "content too long");
        return NULL;
    }
    while (cur_len < total_len) {
        received = httpd_req_recv(request, buffer + cur_len, total_len);
        if (received <= 0) {
            /* Respond with 500 Internal Server Error */
            httpd_resp_send_err(request, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to post control value");
            return NULL;
        }
        cur_len += received;
    }
    buffer[total_len] = '\0';
    return buffer;
}
