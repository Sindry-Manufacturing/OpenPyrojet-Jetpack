#ifndef OPENPYROJET_APP_STATE_H
#define OPENPYROJET_APP_STATE_H

#include "config.h"

#include <esp_netif_ip_addr.h>

struct AppState {
    Config config;
    esp_ip4_addr_t ip;
} appState;

enum AppStateChange {
    CONFIG_PINS,
    IP
};

void app_state_changed(enum AppStateChange which);

#endif //OPENPYROJET_APP_STATE_H
