#include "app_state.h"
#include "display_page.h"

void app_state_changed(enum AppStateChange which) {
    switch (which) {
        case CONFIG_PINS:
            // TODO: re-init jetpack_io
            break;
        case IP:
            display_page_notify_changed(CONFIG_MODE);
            display_page_notify_changed(NORMAL_MODE);
            break;
    }
}
