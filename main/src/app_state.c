#include "app_state.h"

#include "display_page.h"
#include "jetpack_io.h"

struct AppState appState;

void app_state_changed(AppStateChange which) {
    switch (which) {
        case CONFIG_PINS:
            jetpack_io_init(&appState.config);
            break;
        case IP:
            display_page_notify_changed(CONFIG_MODE);
            display_page_notify_changed(NORMAL_MODE);
            break;
    }
}
