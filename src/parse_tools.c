#include "util_parse.h"

bool Json_GetString(cJSON* obj, const char* key, char* out)
{
  size_t max_len = MAX_NAME_LEN -1;

  cJSON* item = cJSON_GetObjectItem(obj, key);
  if (item && cJSON_IsString(item) && out) {
    strncpy(out, item->valuestring, max_len);
    out[max_len] = '\0';
    return true;
  }
  out[0] = '\0';
  return false;
}

int Json_GetInt(cJSON* obj, const char* key, int default_val)
{
  cJSON* item = cJSON_GetObjectItem(obj, key);
  return cJSON_IsNumber(item) ? item->valueint : default_val;
}

float Json_GetFloat(cJSON* obj, const char* key, float default_val)
{
  cJSON* item = cJSON_GetObjectItem(obj, key);
  return cJSON_IsNumber(item) ? (float)item->valuedouble : default_val;
}

bool Json_GetBool(cJSON* obj, const char* key){
  cJSON* item = cJSON_GetObjectItem(obj, key);
  if (!cJSON_IsBool(item))
   return false;

  return item->valueint != 0;;
}


