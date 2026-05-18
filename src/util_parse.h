#ifndef __UTIL_PARSE__
#define __UTIL_PARSE__

#include "game_helpers.h"
#include "game_strings.h"
char* Json_GetString(cJSON*, const char*, char* out);
int Json_GetInt(cJSON*, const char*, int default_val);
float Json_GetFloat(cJSON*, const char*, float default_val);
bool Json_GetBool(cJSON* obj, const char* key);
#endif
