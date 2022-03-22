#include "file.h"
#include <sys/stat.h>
#include <esp_log.h>

static const char* TAG = "file";

bool file_exists(const char* filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
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
        ESP_LOGE(TAG, "failed to open target file %s", fromPath);
        return -1;
    }

    FILE* targetFile = fopen(toPath, "w");
    if (targetFile == NULL) {
        ESP_LOGE(TAG, "failed to open target file %s", toPath);
        fclose(sourceFile);
        return -1;
    }

    // Copy all bytes from source to target
    int character;
    long size = 0;
    while ((character = fgetc(sourceFile)) != EOF) {
        fputc(character, targetFile);
        size++;
    }

    ESP_LOGI(TAG, "copied %s into %s", fromPath, toPath);

    fclose(sourceFile);
    fclose(targetFile);

    return size;
}
