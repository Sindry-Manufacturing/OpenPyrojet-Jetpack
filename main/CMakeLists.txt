idf_component_register(SRCS "src/app_state.c"
    "src/config.c"
    "src/config_json.c"
    "src/display.c"
    "src/display_config.c"
    "src/display_config_json.c"
    "src/display_page.c"
    "src/file.c"
    "src/fs_init.c"
    "src/jetpack_io.c"
    "src/json.c"
    "src/main.c"
    "src/message_json.c"
    "src/rest_handler_wildcard.c"
    "src/rest_handler_wifi.c"
    "src/rest_server.c"
    "src/rest_utils.c"
    "src/state_machine.c"
    "src/websocket_server.c"
    "src/websocket_server_start.c"
    "src/websocket_handler.c"
    "src/wifi_ap.c"
    "src/wifi_config.c"
    "src/wifi_config_json.c"
    "src/wifi_connect.c"
    "src/ssd1306/ssd1306.c"
    "src/ssd1306/ssd1306_i2c.c"
    "src/ssd1306/ssd1306_spi.c"
    INCLUDE_DIRS ".")

set(WEB_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/www")
spiffs_create_partition_image(www ${WEB_SRC_DIR} FLASH_IN_PROJECT)

set(CONFIG_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/config")
spiffs_create_partition_image(config ${CONFIG_SRC_DIR} FLASH_IN_PROJECT)
