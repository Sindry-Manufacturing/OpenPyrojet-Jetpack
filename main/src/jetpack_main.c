#include "jetpack_main.h"
#include "config.h"
#include "esp_log.h"

const char* TAG = "JETPACK";

JetpackSpecifications jetpackSpecifications;
JetpackParameters jetpackParameters;

void jetpack_init() {
    // Specifications (constants)
    jetpackSpecifications.nozzleCount = NOZZLE_COUNT;
    // Parameters (changeable)
    jetpackParameters.heatingDuration = HEATING_DURATION;
    jetpackParameters.triggerDelay = TRIGGER_DELAY;
}

void jetpack_fire(uint8 nozzleId) {
    int maxId = jetpackSpecifications.nozzleCount - 1;
    if (nozzleId > 0 && nozzleId < maxId) {
        ESP_LOGI(TAG, "Firing nozzle %d", nozzleId);
        // TODO: implement
    }
}
