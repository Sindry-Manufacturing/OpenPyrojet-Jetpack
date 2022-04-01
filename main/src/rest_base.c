#include "rest_base.h"

char* rest_read_buffer(httpd_req_t* request) {
    int contentLength = request->content_len;
    int currentLength = 0;
    char* buffer = ((rest_server_context_t *)(request->user_ctx))->scratch;
    int received = 0;
    if (contentLength >= SCRATCH_BUFSIZE) {
        // Respond with 500 Internal Server Error
        httpd_resp_send_err(request, HTTPD_500_INTERNAL_SERVER_ERROR, "content too long");
        return NULL;
    }
    while (currentLength < contentLength) {
        received = httpd_req_recv(request, buffer + currentLength, contentLength);
        if (received <= 0) {
            // Respond with 500 Internal Server Error
            httpd_resp_send_err(request, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to post control value");
            return NULL;
        }
        currentLength += received;
    }
    buffer[contentLength] = '\0';
    return buffer;
}
