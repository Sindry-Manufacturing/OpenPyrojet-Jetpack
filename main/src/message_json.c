#include "message_json.h"

#include <string.h>

/**
 * Reads a message from JSON.
 * Returns the cJSON object so it can be used to continue parsing the Message data.
 *
 * @param message target
 * @param jsonText to parse
 * @return the root json, which MUST be freed by the caller after Message is processed
 */
cJSON* message_from_json(Message* message, const char* jsonText) {
    cJSON* root = cJSON_Parse(jsonText);

    cJSON* typeJson = cJSON_GetObjectItemCaseSensitive(root, "type");
    if (cJSON_IsString(typeJson)) {
        if (strlen(typeJson->valuestring) < sizeof(message->type)) {
            strcpy(message->type, typeJson->valuestring);
        }
    } else {
        strcpy(message->type, "unknown");
    }

    message->data = cJSON_GetObjectItem(root, "data");

    return root;
}

cJSON* message_json_object(const char* type, cJSON* data) {
    cJSON* root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "type", type);
    cJSON_AddItemToObject(root, "data", data);
    return root;
}
