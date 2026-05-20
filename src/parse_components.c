#include "component_define.h"
#include "game_control.h"
#include "util_parse.h"
#include "tool_lookup.h"

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
    
    char t_name[MAX_NAME_LEN];
    Json_GetString(action_json, "action", t_name);

    cJSON* key_json;
    cJSON_ArrayForEach(key_json, keys_json){
      int key = key_json->valueint;

      action_key_t *a = GameCalloc("ParseInputComponent", 1, sizeof(action_key_t));
      a->key = key;
      strcpy(a->name, aname->valuestring);
      a->type = StringToAction(t_name);

      out->action_keys[keys++] = *a; 
    }
  }

  return keys > 0;
}

bool ParseRigidBodyComponent(cJSON* j, rigid_body_t* out){
  if(!j)
    return false;

  int w = Json_GetInt(j, "width", 0);
  int h = Json_GetInt(j, "height", 0);

  Vector2 size = VEC_NEW(w,h);
  RigidBodySetBounds(out, size);

  char ename[MAX_NAME_LEN];
  Json_GetString(j, "event", ename);

  out->on_coll = StringToPhysEvent(ename);

  char sname[MAX_NAME_LEN];
  Json_GetString(j, "shape", sname);

  out->bounds.shape = StringToShape(sname);

  return out->bounds.shape > SHAPE_NONE;
}

bool ParseForceComponent(cJSON* j, force_t* out){
  if(!j)
    return false;

  out->speed = Json_GetFloat(j, "speed", 0);
  out->threshold = Json_GetFloat(j, "threshold", 0);
  out->max_velocity = Json_GetFloat(j, "max", 0);
  
  float fric = Json_GetFloat(j, "frict", 0);
 
  out->friction = VEC_NEW(fric, fric);

  char tname[MAX_NAME_LEN];
  Json_GetString(j, "type", tname);

  out->type = StringToForce(tname);

  char rname[MAX_NAME_LEN];
  Json_GetString(j, "react", rname);

  out->react = StringToReaction(rname);

  char ename[MAX_NAME_LEN];
  Json_GetString(j, "event", ename);

  out->event = StringToPhysEvent(ename);


  return out->type > FORCE_NONE;
}

bool ParseTrackingComponent(cJSON* j, tracking_t* out){
  if(!j)
    return false;

  char m_name[MAX_NAME_LEN];
  Json_GetString(j, "mode", m_name);

  out->ctx = *InitCameraContext(StringToCameraMode(m_name));

  return out->ctx.mode > 0;
}
