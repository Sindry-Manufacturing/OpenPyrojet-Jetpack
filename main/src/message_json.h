#ifndef OPENPYROJET_MESSAGE_JSON_H
#define OPENPYROJET_MESSAGE_JSON_H

#include "message.h"
#include <cJSON.h>

cJSON* message_from_json(Message* message, const char* text);

#endif //OPENPYROJET_MESSAGE_JSON_H
