#include "file.h"

#include <sys/stat.h>
#include <esp_log.h>
#include <unistd.h>

static const char* TAG = "file";

bool file_exists(const char* filename) {
    return access(filename, F_OK) == 0;
}

long file_get_size(FILE* file) {
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);
    return fileSize;
}

long file_copy(const char* fromPath, const char* toPath) {
    FILE* sourceFile = fopen(fromPath, "r");
    if (sourceFile == NULL) {
        ESP_LOGE(TAG, "failed to open source file %s", fromPath);
        return -1;
    }

    FILE* targetFile = fopen(toPath, "w");
    if (targetFile == NULL) {
        ESP_LOGE(TAG, "failed to open target file %s", toPath);
        fclose(sourceFile);
        return -1;
    }

    // Copy all bytes from source to target
    char buffer[1024];
    size_t bytes;
    size_t totalBytes = 0;
    while (0 < (bytes = fread(buffer, 1, sizeof(buffer), sourceFile)))
      totalBytes += fwrite(buffer, 1, bytes, targetFile);
    ESP_LOGI(TAG, "copied %s into %s (%u bytes)", fromPath, toPath, totalBytes);

    fclose(sourceFile);
    fclose(targetFile);

    return totalBytes;
}
