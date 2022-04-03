#ifndef OPENPYROJET_JETPACK_IO_H
#define OPENPYROJET_JETPACK_IO_H

#include <esp_err.h>

#include "types.h"
#include "config.h"

esp_err_t jetpack_io_init(const Config* config);
void jetpack_io_fire(uint8 nozzleId); // nozzle trigger placeholder

#endif //OPENPYROJET_JETPACK_IO_H
