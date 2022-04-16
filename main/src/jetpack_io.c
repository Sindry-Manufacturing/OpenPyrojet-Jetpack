#include "jetpack_io.h"

#include <esp_log.h>
#include <driver/gpio.h>

#include "app_state.h"
#include "time.h"

static const char* TAG = "jetpack";

static uint64 gpio_pin_bit_mask(const Config* config) {
    uint64 pinBitMask = 0ULL;
    for (int pinIndex = 0; pinIndex < config->nozzleCount; ++pinIndex) {
        uint8 pinNumber = config->nozzlePins[pinIndex];
        pinBitMask = pinBitMask | (1ULL << pinNumber);
    }
    return pinBitMask;
}

esp_err_t jetpack_io_init(const Config* config) {
    uint64 pinBitMask = gpio_pin_bit_mask(config);
    if (pinBitMask != 0) {
        gpio_config_t gpio = {
            .intr_type = GPIO_INTR_DISABLE,
            .mode = GPIO_MODE_OUTPUT,
            .pin_bit_mask = pinBitMask,
            .pull_down_en = 0, // disabled
            .pull_up_en = 0 // disabled
        };
        return gpio_config(&gpio);
    } else {
        ESP_LOGW(TAG, "no nozzle pins configured");
        return ESP_OK;
    }
}

void jetpack_io_fire(uint8 nozzleId) {
    int maxId = appState.config.nozzleCount - 1;
    if (nozzleId <= maxId) {
        uint8 nozzlePin = appState.config.nozzlePins[nozzleId];
        ESP_LOGI(TAG, "Firing nozzle %d at pin %d for %d µs", nozzleId, nozzlePin, appState.config.heatingDuration);
        gpio_set_level(nozzlePin, 1);
        ets_delay_us(appState.config.heatingDuration);
        gpio_set_level(nozzlePin, 0);
    }  else {
        ESP_LOGE(TAG, "Invalid nozzle index: %d (max %d)", nozzleId, maxId);
    }
}
