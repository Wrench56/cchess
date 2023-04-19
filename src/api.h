#pragma once

#include <cjson/cJSON.h>

cJSON* http_request(char* api_key, char* url, char* post);
void read_game_stream(void* game_state_ptr);