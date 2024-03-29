#ifndef OPENPYROJET_APP_STATE_H
#define OPENPYROJET_APP_STATE_H

#include "config.h"

#include <esp_netif_ip_addr.h>

extern struct AppState appState;

struct AppState {
    Config config;
    esp_ip4_addr_t ip;
};

typedef enum {
    CONFIG_PINS,
    IP
} AppStateChange;

void app_state_changed(AppStateChange which);

#endif //OPENPYROJET_APP_STATE_H
