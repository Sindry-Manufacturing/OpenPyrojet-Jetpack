idf_component_register(SRCS "src/main.c"
    "src/config.c"
    "src/config_json.c"
    "src/file.c"
    "src/message_json.c"
    "src/wifi_connect.c"
    "src/fs_init.c"
    "src/rest_base.c"
    "src/rest_server.c"
    "src/rest_wildcard.c"
    "src/rest_system_info.c"
    "src/rest_wifi.c"
    "src/wifi_config_json.c"
    "src/wifi_ap.c"
    "src/jetpack_main.c"
    "src/websocket_server.c"
    "src/websocket_server_start.c"
    "src/websocket_handler.c"
    INCLUDE_DIRS ".")

set(WEB_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/www")
spiffs_create_partition_image(www ${WEB_SRC_DIR} FLASH_IN_PROJECT)

set(CONFIG_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/config")
spiffs_create_partition_image(config ${CONFIG_SRC_DIR} FLASH_IN_PROJECT)
