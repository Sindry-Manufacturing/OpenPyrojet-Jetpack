#include "display_page.h"

#include "state_machine.h"
#include "display.h"
#include "app_state.h"

#include <string.h>
#include <stddef.h>

static struct StateMachine* machine = NULL;

static int stateIds[2];

const char* pageConfigMode = "config-mode";
const char* pageNormalMode = "normal-mode";

void display_show_wifi_ap_mode();
void display_show_wifi_normal_mode();

static void display_page_ensure_initialized() {
    if (machine != NULL) {
        return;
    }

    machine = state_machine_create("display-state-machine", 2);

    stateIds[CONFIG_MODE] = state_machine_add_state(
        machine,
        pageConfigMode,
        NULL,
        &display_show_wifi_ap_mode,
        NULL
    );

    stateIds[NORMAL_MODE] = state_machine_add_state(
        machine,
        pageNormalMode,
        NULL,
        &display_show_wifi_normal_mode,
        NULL
    );
}

void display_page_show(DisplayPage state) {
    display_page_ensure_initialized();

    switch (state) {
        case CONFIG_MODE:
            state_machine_enter(machine, stateIds[CONFIG_MODE]);
            break;
        case NORMAL_MODE:
            state_machine_enter(machine, stateIds[NORMAL_MODE]);
            break;
    }
}

void display_page_notify_changed(DisplayPage state) {
    display_page_ensure_initialized();

    switch (state) {
        case CONFIG_MODE:
            state_machine_update(machine, stateIds[CONFIG_MODE]);
            break;
        case NORMAL_MODE:
            state_machine_update(machine, stateIds[NORMAL_MODE]);
            break;
    }
}

// region Page implementations

static void ip_to_string(esp_ip4_addr_t ip, char* target) {
    snprintf(target, 128, "%hu.%hu.%hu.%hu",
             esp_ip4_addr1_16(&ip), \
        esp_ip4_addr2_16(&ip), \
        esp_ip4_addr3_16(&ip), \
        esp_ip4_addr4_16(&ip)
    );
}

static void display_show_header(SSD1306_t* display) {
    ssd1306_clear_screen(display, false);
    ssd1306_display_text(display, 0, "  OpenPyrojet   ", 16, true);
}

void display_show_wifi_ap_mode() {
    if (!display_is_initialized()) {
        return;
    }

    SSD1306_t* display = display_get();
    display_show_header(display);

    const char* wifiSsid = appState.config.wifi.ssid;
    const char* wifiPassword = appState.config.wifi.password;
    ssd1306_display_text(display, 1, "Wifi SSID:", 10, false);
    ssd1306_display_text_with_offset(display, 2, wifiSsid, strlen(wifiSsid), false, 8);
    ssd1306_display_text(display, 3, "Wifi password:", 14, false);
    ssd1306_display_text_with_offset(display, 4, wifiPassword, strlen(wifiPassword), false, 8);
    ssd1306_display_text(display, 5, "http://", 7, false);

    if (appState.ip.addr != 0) {
        char ipString[128];
        ip_to_string(appState.ip, ipString);
        ssd1306_display_text_with_offset(display, 6, ipString, strlen(ipString), false, 8);
    }
}

void display_show_wifi_normal_mode() {
    if (!display_is_initialized()) {
        return;
    }

    SSD1306_t* display = display_get();

    display_show_header(display);

    ssd1306_display_text(display, 2, "IP:", 3, false);
    if (appState.ip.addr != 0) {
        char target[128];
        ip_to_string(appState.ip, target);
        ssd1306_clear_line(display, 3, false);
        ssd1306_display_text_with_offset(display, 3, target, strlen(target), false, 8);
    } else {
        ssd1306_display_text(display, 3, "[Connecting ...]", 16, false);
    }
}

// endregion Page implementations
