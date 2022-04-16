#include "rest_handler_wildcard.h"

#include <fcntl.h>

#include "esp_log.h"

#define FILE_PATH_MAX (ESP_VFS_PATH_MAX + 128)
#define CHECK_FILE_EXTENSION(filename, ext) (strcasecmp(&filename[strlen(filename) - strlen(ext)], ext) == 0)

/* Set HTTP response content type according to file extension */
static esp_err_t set_content_type_from_file(httpd_req_t* request, const char* filepath) {
    const char *type = "text/plain";
    if (CHECK_FILE_EXTENSION(filepath, ".html")) {
        type = "text/html";
    } else if (CHECK_FILE_EXTENSION(filepath, ".js")) {
        type = "application/javascript";
    } else if (CHECK_FILE_EXTENSION(filepath, ".css")) {
        type = "text/css";
    } else if (CHECK_FILE_EXTENSION(filepath, ".png")) {
        type = "image/png";
    } else if (CHECK_FILE_EXTENSION(filepath, ".jpg")) {
        type = "image/jpg";
    } else if (CHECK_FILE_EXTENSION(filepath, ".jpeg")) {
        type = "image/jpg";
    } else if (CHECK_FILE_EXTENSION(filepath, ".ico")) {
        type = "image/x-icon";
    } else if (CHECK_FILE_EXTENSION(filepath, ".svg")) {
        type = "text/xml";
    }
    return httpd_resp_set_type(request, type);
}

/* Send HTTP response with the contents of the requested file */
static esp_err_t rest_common_get_handler(httpd_req_t* request) {
    char filepath[FILE_PATH_MAX];

    rest_server_context_t* restContext = (rest_server_context_t *) request->user_ctx;
    strlcpy(filepath, restContext->base_path, sizeof(filepath));
    if (request->uri[strlen(request->uri) - 1] == '/') {
        strlcat(filepath, "/index.html", sizeof(filepath));
    } else {
        strlcat(filepath, request->uri, sizeof(filepath));
    }
    int fileDescriptor = open(filepath, O_RDONLY, 0);
    if (fileDescriptor == -1) {
        ESP_LOGE(REST_TAG, "Failed to open file : %s", filepath);
        /* Respond with 500 Internal Server Error */
        httpd_resp_send_err(request, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to read existing file");
        return ESP_FAIL;
    }

    set_content_type_from_file(request, filepath);

    char* chunk = restContext->scratch;
    ssize_t bytesRead;
    do {
        /* Read file in chunks into the scratch buffer */
        bytesRead = read(fileDescriptor, chunk, SCRATCH_BUFFER_SIZE);
        if (bytesRead == -1) {
            ESP_LOGE(REST_TAG, "Failed to read file : %s", filepath);
        } else if (bytesRead > 0) {
            /* Send the buffer contents as HTTP response chunk */
            if (httpd_resp_send_chunk(request, chunk, bytesRead) != ESP_OK) {
                close(fileDescriptor);
                ESP_LOGE(REST_TAG, "File sending failed!");
                /* Abort sending file */
                httpd_resp_sendstr_chunk(request, NULL);
                /* Respond with 500 Internal Server Error */
                httpd_resp_send_err(request, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to send file");
                return ESP_FAIL;
            }
        }
    } while (bytesRead > 0);
    /* Close file after sending complete */
    close(fileDescriptor);
    ESP_LOGI(REST_TAG, "File sending complete");
    /* Respond with an empty chunk to signal HTTP response completion */
    httpd_resp_send_chunk(request, NULL, 0);
    return ESP_OK;
}

void register_wildcard_uri_handler(
    httpd_handle_t server,
    rest_server_context_t* restContext
) {
    /* URI handler for getting web server files */
    httpd_uri_t commonGetUri = {
        .uri = "/*",
        .method = HTTP_GET,
        .handler = rest_common_get_handler,
        .user_ctx = restContext
    };
    httpd_register_uri_handler(server, &commonGetUri);
}
