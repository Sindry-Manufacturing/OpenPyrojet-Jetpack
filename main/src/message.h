#ifndef OPENPYROJET_MESSAGE_H
#define OPENPYROJET_MESSAGE_H

#include <cJSON.h>

enum MessageType {
    UNKNOWN,
    GET_CONFIG,
    PUT_CONFIG,
    FIRE_NOZZLE,
};

/**
 * WebSocket JSON message. It's formatted as follows:
 * { type: "GET_CONFIG", data: ... }
 */
typedef struct {
    enum MessageType type;
    cJSON* data;
} Message;


#endif //OPENPYROJET_MESSAGE_H
