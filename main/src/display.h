#ifndef OPENPYROJET_DISPLAY_H
#define OPENPYROJET_DISPLAY_H

#include <esp_netif.h>

#include "u8g2.h"
//#include "ssd1306/ssd1306.h"
#include "display_config.h"

void display_init(const DisplayConfig* config);
bool display_is_initialized();
u8g2_t* display_get();

#endif //OPENPYROJET_DISPLAY_H
