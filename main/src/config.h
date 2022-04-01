#ifndef OPENPYROJET_CONFIG_H
#define OPENPYROJET_CONFIG_H

#include <esp_err.h>

#include "types.h"
#include "wifi_config.h"
#include "display_config.h"

#define CONFIG_FILE_SIZE_MAX 4096 // max config size in bytes
#define NOZZLE_COUNT_MAX 16

// Constants
typedef struct {
    WifiConfig wifi;
    DisplayConfig display;
    uint8 nozzleCount;
    uint8 nozzlePins[NOZZLE_COUNT_MAX];
    uint32 heatingDuration; // duration of filament heating in µs
    uint32 triggerDelay; // minimum delay between consecutive ejections in µs
} Config;

extern Config config;

/**
 * Loads the user configuration from the filesystem.
 * If a user configuration is not found, it is first copied from the default one.
 */
bool config_init();

/**
 * Save the configuration into the user configuration file.
 */
bool config_save(const Config* config);

#endif //OPENPYROJET_CONFIG_H
