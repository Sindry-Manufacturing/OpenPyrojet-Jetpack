#ifndef OPENPYROJET_JETPACK_MAIN_H
#define OPENPYROJET_JETPACK_MAIN_H

#include <esp_err.h>

#include "types.h"

esp_err_t jetpack_init();
void jetpack_fire(uint8 nozzleId); // nozzle trigger placeholder

#endif //OPENPYROJET_JETPACK_MAIN_H
