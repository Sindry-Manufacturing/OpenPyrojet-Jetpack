#ifndef OPENPYROJET_DISPLAY_H
#define OPENPYROJET_DISPLAY_H

#include <esp_netif.h>

#include "display_config.h"

void display_init(const DisplayConfig* config);
void display_show_wifi_ap_mode();
void display_show_wifi_normal_mode();

#endif //OPENPYROJET_DISPLAY_H
