#include "sdkconfig.h"
#include "driver/gpio.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_log.h"
#include "mdns.h"
#include "lwip/apps/netbiosns.h"
#include "protocol_examples_common.h"
#include "jetpack_main.h"
#include "rest_server.h"

#include "fs_init.h"
#include "config.h"

#define MDNS_INSTANCE "Jetpack Server"
#define CONFIG_EXAMPLE_MDNS_HOST_NAME "pyrojet"

static void init_mdns(void) {
    mdns_init();
    mdns_hostname_set(CONFIG_EXAMPLE_MDNS_HOST_NAME);
    mdns_instance_name_set(MDNS_INSTANCE);

    mdns_txt_item_t serviceTxtData[] = {
        {"board", "esp32"},
        {"path",  "/"}
    };

    ESP_ERROR_CHECK(mdns_service_add(
        "Jetpack-WebServer",
        "_http",
        "_tcp",
        80,
        serviceTxtData,
        sizeof(serviceTxtData) / sizeof(serviceTxtData[0])
    ));
}

void app_main(void) {
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(fs_init());

    ESP_ERROR_CHECK(jetpack_init());

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    init_mdns();
    netbiosns_init();
    netbiosns_set_name(CONFIG_EXAMPLE_MDNS_HOST_NAME);
    ESP_ERROR_CHECK(example_connect());
    ESP_ERROR_CHECK(rest_server_start(MOUNT_POINT_WWW));
}
