#ifndef OPENPYROJET_WIFI_AP_H
#define OPENPYROJET_WIFI_AP_H

#include <stdbool.h>

#define CONFIG_AP_WIFI_SSID "OpenPyrojet"
#define CONFIG_AP_WIFI_PASSWORD "opensesame"

void wifi_ap_init();
bool wifi_ap_start();

const char* wifi_ap_get_ssid();
const char* wifi_ap_get_password();

#endif //OPENPYROJET_WIFI_AP_H
