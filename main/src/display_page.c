#include "display_page.h"

#include "state_machine.h"
#include "display.h"

#include <stddef.h>

static struct StateMachine* machine = NULL;

static int stateIds[2];

const char* pageConfigMode = "config-mode";
const char* pageNormalMode = "normal-mode";

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

void display_page_show(enum DisplayPage state) {
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

void display_page_notify_changed(enum DisplayPage state) {
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
