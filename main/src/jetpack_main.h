#ifndef OPENPYROJET_JETPACK_MAIN_H
#define OPENPYROJET_JETPACK_MAIN_H

#include "types.h"

#define NOZZLE_COUNT_MAX 16

// Constants
typedef struct {
    uint8 nozzleCount;
    uint8 nozzlePins[NOZZLE_COUNT_MAX];
} JetpackSpecifications;

// Changeable settings
typedef struct {
    uint8 heatingDuration; // duration of filament heating in ms
    uint16 triggerDelay; // minimum delay between consecutive ejections in ms
} JetpackParameters;

extern JetpackSpecifications jetpackSpecifications;

extern JetpackParameters jetpackParameters;

void jetpack_init();
void jetpack_fire(uint8 nozzleId); // nozzle trigger placeholder

#endif //OPENPYROJET_JETPACK_MAIN_H
