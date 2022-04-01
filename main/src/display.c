#include "display.h"

#include <string.h>

#include "ssd1306/ssd1306.h"

SSD1306_t dev;
bool displayStarted = false;

static void ip_to_string(esp_ip4_addr_t ip, char* target) {
    snprintf(target, 128, "%hu.%hu.%hu.%hu",
        esp_ip4_addr1_16(&ip), \
        esp_ip4_addr2_16(&ip), \
        esp_ip4_addr3_16(&ip), \
        esp_ip4_addr4_16(&ip)
    );
}

void display_init(const DisplayConfig* config) {
    i2c_master_init(&dev, config->pinSda, config->pinScl, config->pinReset);
    displayStarted = ssd1306_init(&dev, config->pixelWidth, config->pixelHeight) == ESP_OK;

    if (displayStarted) {
        ssd1306_clear_screen(&dev, false);
        ssd1306_contrast(&dev, 0xff);
    }
}

void display_show_header() {
    ssd1306_display_text(&dev, 0, "  OpenPyrojet   ", 16, true);
}

void display_show_wifi_ap_mode(const char* wifiSsid, const char* wifiPassword, esp_ip4_addr_t ip) {
    if (!displayStarted) {
        return;
    }

    ssd1306_display_text(&dev, 1, "Wifi SSID:", 10, false);
    ssd1306_display_text_with_offset(&dev, 2, wifiSsid, strlen(wifiSsid), false, 8);
    ssd1306_display_text(&dev, 3, "Wifi password:", 14, false);
    ssd1306_display_text_with_offset(&dev, 4, wifiPassword, strlen(wifiPassword), false, 8);
    ssd1306_display_text(&dev, 5, "http://", 7, false);

    if (ip.addr != 0) {
        char ipString[128];
        ip_to_string(ip, ipString);
        ssd1306_display_text_with_offset(&dev, 6, ipString, strlen(ipString), false, 8);
    }
}

void display_show_wifi_normal_mode(esp_ip4_addr_t ip) {
    if (!displayStarted) {
        return;
    }

    ssd1306_display_text(&dev, 2, "IP:", 3, false);
    if (ip.addr != 0) {
        char target[128];
        ip_to_string(ip, target);
        ssd1306_clear_line(&dev, 3, false);
        ssd1306_display_text_with_offset(&dev, 3, target, strlen(target), false, 8);
    } else {
        ssd1306_display_text(&dev, 3, "[Connecting ...]", 16, false);
    }
}
