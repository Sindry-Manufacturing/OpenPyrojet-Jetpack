#include "jetpack_main.h"

#include <esp_log.h>
#include <driver/gpio.h>

#include "config.h"
#include "time.h"

const char* TAG = "jetpack";

uint64 gpio_pin_bit_mask(const Config* config) {
    uint64 pinBitMask = 0ULL;
    for (int pinIndex = 0; pinIndex < config->nozzleCount; ++pinIndex) {
        uint8 pinNumber = config->nozzlePins[pinIndex];
        pinBitMask = pinBitMask | (1ULL << pinNumber);
    }
    return pinBitMask;
}


void jetpack_init_gpio(Config* config) {
    uint64 pinBitMask = gpio_pin_bit_mask(config);
    gpio_config_t gpioConfig;
    gpioConfig.intr_type = GPIO_INTR_DISABLE;
    gpioConfig.mode = GPIO_MODE_OUTPUT;
    gpioConfig.pin_bit_mask = pinBitMask;
    gpioConfig.pull_down_en = 0; // disabled
    gpioConfig.pull_up_en = 0; // disabled
    gpio_config(&gpioConfig);
}

esp_err_t jetpack_init() {
    if (!config_init()) {
        ESP_LOGE(TAG, "config_init() failed");
        return ESP_FAIL;
    }

    jetpack_init_gpio(&config);

    return ESP_OK;
}

void jetpack_fire(uint8 nozzleId) {
    int maxId = config.nozzleCount - 1;
    if (nozzleId <= maxId) {
        uint8 nozzlePin = config.nozzlePins[nozzleId];
        ESP_LOGI(TAG, "Firing nozzle %d at pin %d for %d µs", nozzleId, nozzlePin, config.heatingDuration);
        gpio_set_level(nozzlePin, 1);
        usleep(config.heatingDuration);
        gpio_set_level(nozzlePin, 0);
    }  else {
        ESP_LOGE(TAG, "Invalid nozzle index: %d (max %d)", nozzleId, maxId);
    }
}
