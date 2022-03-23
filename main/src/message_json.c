#include "message_json.h"
#include <stdlib.h>
#include <string.h>

static int message_type_from_string(const char* text) {
    if (strcmp(text, "GET_CONFIG") == 0) {
        return GET_CONFIG;
    } else if (strcmp(text, "PUT_CONFIG") == 0) {
        return PUT_CONFIG;
    } else if (strcmp(text, "FIRE_NOZZLE") == 0) {
        return FIRE_NOZZLE;
    } else {
        return UNKNOWN;
    }
}

/**
 * @param message
 * @param text
 * @return the root json, which MUST be freed after Message is processed
 */
cJSON* message_from_json(Message* message, const char* text) {
    cJSON* root = cJSON_Parse(text);

    cJSON* typeJson = cJSON_GetObjectItemCaseSensitive(root, "type");
    if (cJSON_IsString(typeJson)) {
        message->type = message_type_from_string(typeJson->valuestring);
    } else {
        message->type = UNKNOWN;
    }

    message->data = cJSON_GetObjectItem(root, "data");

    return root;
}
