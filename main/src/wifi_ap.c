/**
 * The code below is based on https://github.com/nopnop2002/esp-idf-wifi-apsta
 * See THIRD-PARTY.md
 */

#include "wifi_ap.h"

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"

#include "display.h"

#define CONFIG_AP_WIFI_CONNECTION_LIMIT 4

static const char* TAG = "wifi-ap";
static EventGroupHandle_t wifi_event_group;
const int CONNECTED_BIT = BIT0;

/**
 * @return a random number in the range of [1, 13]
 */
static uint8_t random_wifi_channel() {
    return (abs(rand()) % 13) + 1;
}

static void event_handler(
    void* arg,
    esp_event_base_t eventBase,
    int32_t eventId,
    void* eventData
) {
    if (eventBase == WIFI_EVENT && eventId == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGI(TAG, "WIFI_EVENT_STA_DISCONNECTED");
        esp_wifi_connect();
        xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
    } else if (eventBase == IP_EVENT && eventId == IP_EVENT_STA_GOT_IP) {
        ESP_LOGI(TAG, "IP_EVENT_STA_GOT_IP");
        xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
    }
}

void wifi_ap_init() {
    esp_log_level_set("wifi", ESP_LOG_WARN);
    ESP_ERROR_CHECK(esp_netif_init());
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_t* apNetif = esp_netif_create_default_wifi_ap();
    assert(apNetif);
    esp_netif_t* staNetif = esp_netif_create_default_wifi_sta();
    assert(staNetif);
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));

    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_NULL));
    ESP_ERROR_CHECK(esp_wifi_start());
}

bool wifi_ap_start() {
    wifi_config_t wifiConfig = { 0 };
    strcpy((char*)wifiConfig.ap.ssid, CONFIG_AP_WIFI_SSID);
    strcpy((char*)wifiConfig.ap.password, CONFIG_AP_WIFI_PASSWORD);
    wifiConfig.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;
    wifiConfig.ap.ssid_len = strlen(CONFIG_AP_WIFI_SSID);
    wifiConfig.ap.max_connection = CONFIG_AP_WIFI_CONNECTION_LIMIT;
    wifiConfig.ap.channel = random_wifi_channel();
    if (strlen(CONFIG_AP_WIFI_PASSWORD) == 0) {
        wifiConfig.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifiConfig));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(
        TAG,
        "started with SSID=%s password=%s channel=%d",
         wifiConfig.ap.ssid,
         wifiConfig.ap.password,
         wifiConfig.ap.channel
     );

    esp_netif_t* netif = NULL;
    esp_netif_ip_info_t ip;
    for (int i = 0; i < esp_netif_get_nr_of_ifs(); ++i) {
        netif = esp_netif_next(netif);
        if (strcmp(esp_netif_get_desc(netif), "ap") == 0) {
            ESP_ERROR_CHECK(esp_netif_get_ip_info(netif, &ip));
            ESP_LOGI(TAG, "Access point IPv4 address: " IPSTR, IP2STR(&ip.ip));
            display_show_wifi_ap_mode(CONFIG_AP_WIFI_SSID, CONFIG_AP_WIFI_PASSWORD, ip.ip);
#ifdef WIFI_USE_IPV6
            esp_ip6_addr_t ip6[MAX_IP6_ADDRS_PER_NETIF];
            int ip6_addrs = esp_netif_get_all_ip6(netif, ip6);
            for (int j = 0; j < ip6_addrs; ++j) {
                esp_ip6_addr_type_t ipv6_type = esp_netif_ip6_get_addr_type(&(ip6[j]));
                ESP_LOGI(TAG, "Access point IPv6 address: " IPV6STR ", type: %s", IPV62STR(ip6[j]), s_ipv6_addr_types[ipv6_type]);
            }
#endif
        }
   }

    return ESP_OK;
}
