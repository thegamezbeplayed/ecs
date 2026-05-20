#include "component_define.h"
#include "game_control.h"
#include "util_parse.h"

bool ParseObserverComponent(cJSON* j, component_observer_t* out){
  if(!j)
    return false;

  Json_GetString(j, "name", out->name);

  out->type = Json_GetInt(j, "type", 0);

  if(out->type <= 0){
    TraceLog(LOG_WARNING, " === PARSE OBSERVE COMP ===\n Invalid OBSTYPE %i", out->type);
    return false;
  }
  char r_name[MAX_NAME_LEN];
  Json_GetString(j, "comp", r_name);

  out->relation = ComponentGetID(r_name);

  if(out->relation == INVALID_COMPONENT){
    TraceLog(LOG_WARNING, " === PARSE OBSERVE COMP ===\n Component %s invalid or not found",r_name);
    return false;
  }
  cJSON* subjects_json = cJSON_GetObjectItem(j, "subjects");
  if(!cJSON_IsArray(subjects_json)){
    TraceLog(LOG_WARNING, "=== PARSE OBSERVE COMP ===\nsubjects array not found or invalid");
    return false;
  }
  int s_num = 0;
  cJSON* s;
  cJSON_ArrayForEach(s, subjects_json)
    strcpy(out->subjects[s_num++], s->valuestring);

  out->num_subj = s_num;
  cJSON* listeners_json = cJSON_GetObjectItem(j, "listeners");
  
  int l_num = 0;
  cJSON* l;
  cJSON_ArrayForEach(l, listeners_json)
    strcpy(out->observers[l_num++], l->valuestring);

  return l_num > 0;
}


bool ParseInputComponent(cJSON* j, input_t* out){
  if(!j)
    return false;

  char name[MAX_NAME_LEN];
  Json_GetString(j, "name", name);

  cJSON* actions_json = cJSON_GetObjectItem(j, "actions");
  if(!cJSON_IsArray(actions_json))
    return false;

  int keys = 0;
  cJSON* action_json;
  cJSON_ArrayForEach(action_json, actions_json){
    cJSON* keys_json = cJSON_GetObjectItem(action_json, "keys");
    if(!cJSON_IsArray(keys_json))
      continue;

    cJSON* aname = cJSON_GetObjectItem(action_json, "name");
    cJSON* key_json;
    cJSON_ArrayForEach(key_json, keys_json){
      int key = key_json->valueint;

      action_key_t *a = GameCalloc("ParseInputComponent", 1, sizeof(action_key_t));
      a->key = key;
      strcpy(a->name, aname->valuestring);

      out->action_keys[keys++] = *a; 
    }
  }

  return keys > 0;
}

bool ParseRigidBodyComponent(cJSON* j, rigid_body_t* out){

}

bool ParseForceComponent(cJSON* j, force_t* out){

  return true;
}

