#ifndef OPENPYROJET_DISPLAY_H
#define OPENPYROJET_DISPLAY_H

#include <esp_netif.h>

#include "display_config.h"

void display_init(const DisplayConfig* config);
void display_show_header();
void display_show_wifi_ap_mode(const char* wifiSsid, const char* wifiPassword, esp_ip4_addr_t ip);
void display_show_wifi_normal_mode(esp_ip4_addr_t ip);

#endif //OPENPYROJET_DISPLAY_H
