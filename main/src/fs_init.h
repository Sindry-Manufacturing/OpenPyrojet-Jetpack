#ifndef OPENPYROJET_FS_INIT_H
#define OPENPYROJET_FS_INIT_H

#include "esp_err.h"

#define MOUNT_POINT_WWW "/www"
#define MOUNT_POINT_CONFIG "/config"

esp_err_t fs_init();

#endif //OPENPYROJET_FS_INIT_H
