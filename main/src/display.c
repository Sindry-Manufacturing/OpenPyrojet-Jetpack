#include "display.h"

static SSD1306_t display = {
    ._width = 0,
    ._height = 0
};

static bool displayStarted = false;

void display_init(const DisplayConfig* config) {
    i2c_master_init(&display, config->pinSda, config->pinScl, config->pinReset);
    displayStarted = ssd1306_init(&display, config->pixelWidth, config->pixelHeight) == ESP_OK;

    if (displayStarted) {
        ssd1306_clear_screen(&display, false);
        ssd1306_contrast(&display, 0xff);
    }
}

bool display_is_initialized() {
    return displayStarted;
}

SSD1306_t* display_get() {
    return &display;
}
