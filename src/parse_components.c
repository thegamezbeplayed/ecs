#include "util_parse.h"

bool ParseObserverComponent(cJSON* j, observer_t* out){
  if(!j)
    return false;

  Json_GetString(j, "name", out->name);

  out->type = Json_GetInt(j, "type", 0);

  return out->type > 0;
}
