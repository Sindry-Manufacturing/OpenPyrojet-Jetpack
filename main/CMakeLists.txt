idf_component_register(SRCS "src/main.c"
    "src/rest_base.c"
    "src/rest_server.c"
    "src/rest_wildcard.c"
    "src/jetpack_main.c"
    "src/jetpack_rest.c"
    INCLUDE_DIRS ".")

set(WEB_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/www")
if(EXISTS ${WEB_SRC_DIR}/dist)
    spiffs_create_partition_image(www ${WEB_SRC_DIR}/dist FLASH_IN_PROJECT)
else()
    message(FATAL_ERROR "${WEB_SRC_DIR}/dist doesn't exit. Run 'npm run build' in ${WEB_SRC_DIR}")
endif()
