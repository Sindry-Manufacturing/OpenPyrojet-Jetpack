#ifndef OPENPYROJET_WIFI_AP_H
#define OPENPYROJET_WIFI_AP_H

#include <stdbool.h>

#define CONFIG_AP_WIFI_SSID "OpenPyrojet"
#define CONFIG_AP_WIFI_PASSWORD "openpyrojet"

void wifi_ap_init();
bool wifi_ap_start();

#endif //OPENPYROJET_WIFI_AP_H
