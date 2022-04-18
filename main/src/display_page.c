#include "display_page.h"

#include "state_machine.h"
#include "display.h"
#include "app_state.h"
#include "wifi_ap.h"

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

static void display_show_header(u8g2_t* display) {
    u8g2_DrawStr(display, 30, 8, "OpenPyrojet");
}

void display_show_wifi_ap_mode() {
    if (!display_is_initialized()) {
        return;
    }

    u8g2_t* display = display_get();

    u8g2_SetPowerSave(display, 0);
    u8g2_ClearBuffer(display);

    display_show_header(display);

    int lineHeight = 9;
    int currentY = 8 + lineHeight;
    u8g2_DrawStr(display, 0, currentY, "Wifi SSID:");
    currentY+=lineHeight;
    u8g2_DrawStr(display, 15, currentY, wifi_ap_get_ssid());
    currentY+=lineHeight;
    u8g2_DrawStr(display, 0, currentY, "Wifi Password:");
    currentY+=lineHeight;
    u8g2_DrawStr(display, 15, currentY, wifi_ap_get_password());
    currentY+=lineHeight;

    if (appState.ip.addr != 0) {
        char ipString[128];
        ip_to_string(appState.ip, ipString);
        u8g2_DrawStr(display, 0, currentY, "Web browser:");
        currentY+=lineHeight;
        u8g2_DrawStr(display, 0, currentY, "http://");
        u8g2_DrawStr(display, 42, currentY, ipString);
    }
    u8g2_SendBuffer(display);
}

void display_show_wifi_normal_mode() {
    if (!display_is_initialized()) {
        return;
    }

    u8g2_t* display = display_get();

    u8g2_SetPowerSave(display, 0);
    u8g2_ClearBuffer(display);

    display_show_header(display);

    int lineHeight = 9;
    int currentY = 8 + (lineHeight * 2);
    u8g2_DrawStr(display, 0, currentY, "IP:");
    if (appState.ip.addr != 0) {
        char target[128];
        ip_to_string(appState.ip, target);
        u8g2_DrawStr(display, 20, currentY, target);
    } else {
        u8g2_DrawStr(display, 20, currentY, "[Connecting ...]");
    }
    u8g2_SendBuffer(display);
}

// endregion Page implementations
