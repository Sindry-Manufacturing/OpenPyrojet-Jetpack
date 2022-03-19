#ifndef OPENPYROJET_CONFIG_H
#define OPENPYROJET_CONFIG_H

#include "esp_err.h"
#include "types.h"

#define CONFIG_FILE_SIZE_MAX 4096 // max config size in bytes
#define NOZZLE_COUNT_MAX 16

// Constants
typedef struct {
    uint8 nozzleCount;
    uint8 nozzlePins[NOZZLE_COUNT_MAX];
    uint8 heatingDuration; // duration of filament heating in ms
    uint16 triggerDelay; // minimum delay between consecutive ejections in ms
} Config;

extern Config config;

// Needs to have have exactly NOZZLE_COUNT_MAX entries (see jetpack_main.h)
#endif //OPENPYROJET_CONFIG_H

esp_err_t config_init();
