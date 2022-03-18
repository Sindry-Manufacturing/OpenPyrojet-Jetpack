#include "jetpack_main.h"
#include "config.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "time.h"

const char* TAG = "JETPACK";

JetpackSpecifications jetpackSpecifications;
JetpackParameters jetpackParameters;

uint64 gpio_pin_bit_mask(const JetpackSpecifications* specifications) {
    uint64 pinBitMask = 0ULL;
    for (int pinIndex = 0; pinIndex < specifications->nozzleCount; ++pinIndex) {
        uint8 pinNumber = specifications->nozzlePins[pinIndex];
        pinBitMask = pinBitMask | (1ULL << pinNumber);
    }
    return pinBitMask;
}

uint8 nozzle_count(const JetpackSpecifications* specifications) {
    // Find first 0-value nozzle pin to find total nozzle count
    uint8 nozzleCount = NOZZLE_COUNT_MAX;
    for (int i=0; i < NOZZLE_COUNT_MAX; ++i) {
        if (specifications->nozzlePins[i] == 0) {
            nozzleCount = i;
            break;
        }
    }
    return nozzleCount;
}

void jetpack_init_gpio(JetpackSpecifications* specifications) {
    uint64 pinBitMask = gpio_pin_bit_mask(specifications);
    gpio_config_t gpioConfig;
    gpioConfig.intr_type = GPIO_INTR_DISABLE;
    gpioConfig.mode = GPIO_MODE_OUTPUT;
    gpioConfig.pin_bit_mask = pinBitMask;
    gpioConfig.pull_down_en = 0; // disabled
    gpioConfig.pull_up_en = 0; // disabled
    gpio_config(&gpioConfig);
}

void jetpack_init() {
    uint8 nozzlePins[NOZZLE_COUNT_MAX] = { NOZZLE_GPIO_PINS };
    for (int i=0; i < NOZZLE_COUNT_MAX; ++i) {
        jetpackSpecifications.nozzlePins[i] = nozzlePins[i];
    }

    jetpackSpecifications.nozzleCount = nozzle_count(&jetpackSpecifications);

    // Parameters (changeable)
    jetpackParameters.heatingDuration = HEATING_DURATION;
    jetpackParameters.triggerDelay = TRIGGER_DELAY;

    jetpack_init_gpio(&jetpackSpecifications);
}

void jetpack_fire(uint8 nozzleId) {
    int maxId = jetpackSpecifications.nozzleCount - 1;
    if (nozzleId < maxId) {
        uint8 nozzlePin = jetpackSpecifications.nozzlePins[nozzleId];
        uint32 micros = jetpackParameters.heatingDuration * 1000L;
        ESP_LOGI(TAG, "Firing nozzle %d at pin %d for %d us", nozzleId, nozzlePin, micros);
        gpio_set_level(nozzlePin, 1);
        usleep(micros);
        gpio_set_level(nozzlePin, 0);
    }
}
