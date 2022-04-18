#include "display.h"

#include <esp_log.h>

#include "u8g2_esp32_hal.h"

u8g2_t u8g2; // a structure which will contain all the data for one display

static bool displayStarted = false;

static const char* TAG = "display";

void display_init(const DisplayConfig* config) {
    u8g2_esp32_hal_t u8g2_esp32_hal = U8G2_ESP32_HAL_DEFAULT;
    u8g2_esp32_hal.sda = config->pinSda;
    u8g2_esp32_hal.scl = config->pinScl;
    u8g2_esp32_hal_init(u8g2_esp32_hal);

    u8g2_Setup_ssd1306_i2c_128x64_noname_f(
        &u8g2,
        U8G2_R0,
        //u8x8_byte_sw_i2c,
        u8g2_esp32_i2c_byte_cb,
        u8g2_esp32_gpio_and_delay_cb);  // init u8g2 structure
    u8x8_SetI2CAddress(&u8g2.u8x8, 0x78);

    ESP_LOGI(TAG, "init started");
    u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,

    ESP_LOGD(TAG, "wake up display");
    u8g2_SetPowerSave(&u8g2, 0); // wake up display
    ESP_LOGD(TAG, "clear buffer");
    u8g2_ClearBuffer(&u8g2);
    ESP_LOGD(TAG, "set font");
    u8g2_SetFont(&u8g2, u8g2_font_6x10_tf);
    u8g2_SendBuffer(&u8g2);
    ESP_LOGI(TAG, "init finished");

    displayStarted = true;
}

bool display_is_initialized() {
    return displayStarted;
}

u8g2_t* display_get() {
    return &u8g2;
}
