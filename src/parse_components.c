#include "util_parse.h"

bool ParseObserverComponent(cJSON* j, observer_t* out){
  if(!j)
    return false;

  Json_GetString(j, "name", out->name);

  out->type = Json_GetInt(j, "type", 0);

  cJSON* subjects_json = cJSON_GetObjectItem(j, "subjects");

  cJSON* s;
  cJSON_ArrayForEach(s, subjects_json){
    char sname[MAX_NAME_LEN];
    Json_GetString(s, "name", sname);


  }
  return out->type > 0;
}
