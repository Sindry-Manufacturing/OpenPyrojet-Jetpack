#ifndef OPENPYROJET_MESSAGE_H
#define OPENPYROJET_MESSAGE_H

#include <cJSON.h>

#define INCOMING_MESSAGE_TYPE_GET_CONFIG "get_config"
#define INCOMING_MESSAGE_TYPE_PUT_CONFIG "put_config"
#define INCOMING_MESSAGE_TYPE_FIRE_NOZZLE "fire_nozzle"

#define OUTGOING_MESSAGE_TYPE_CONFIG "config"

/**
 * WebSocket JSON message. It's formatted as follows:
 * { type: "GET_CONFIG", data: ... }
 */
typedef struct {
    char type[32];
    cJSON* data;
} Message;

#endif //OPENPYROJET_MESSAGE_H
