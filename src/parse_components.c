#include "component_define.h"
#include "game_control.h"
#include "util_parse.h"
#include "tool_lookup.h"

bool ParseNameComponent(cJSON* j, name_t* out){
  if(!j)
    return false;

  Json_GetString(j, "display", out->display);
  Json_GetString(j, "name", out->entity);

  return true;
}

bool ParseDebugComponent(cJSON* j, debug_t* out){
  if(!j)
    return false;

  Json_GetString(j, "comp", out->name);
  out->cid = ComponentGetID(out->name);

  return out->cid != INVALID_COMPONENT;
}

bool ParseParticleEmitterComponent(cJSON* j, particle_emitter_t* out){
  if(!j)
    return false;

  char mode_str[MAX_NAME_LEN];
  Json_GetString(j, "mode", mode_str);
  out->mode = StringToEmitMode(mode_str);

  out->layer = Json_GetInt(j, "layer", -1);
  out->active = Json_GetBool(j, "active");

  out->max = Json_GetInt(j, "max_particles", -1);
  out->rate = Json_GetInt(j, "rate", -1);
  out->dur_max = Json_GetInt(j, "lifetime_max", -1);
  out->dur_min = Json_GetInt(j, "lifetime_min", -1);

  float offx = Json_GetFloat(j, "offset_x", 0);
  float offy = Json_GetFloat(j, "offset_y", 0);
  out->offset = VEC_NEW(offx, offy);

  float velocity_minx = Json_GetFloat(j, "velocity_min_x", 0);
  float velocity_miny = Json_GetFloat(j, "velocity_min_y", 0);
  out->acceleration = out->velocity_min = VEC_NEW(velocity_minx, velocity_miny);

  float velocity_maxx = Json_GetFloat(j, "velocity_max_x", 0);
  float velocity_maxy = Json_GetFloat(j, "velocity_max_y", 0);
  out->velocity_max = VEC_NEW(velocity_maxx, velocity_maxy);


  float spawnx = Json_GetFloat(j, "spawn_radius_x", 0);
  float spawny = Json_GetFloat(j, "spawn_radius_y", 0);
  out->spawn_radius = VEC_NEW(spawnx, spawny);

  out->scale_max = Json_GetFloat(j, "scale_max", 1);
  out->scale_min = Json_GetFloat(j, "scale_min", 0);

  out->scale_end = Json_GetFloat(j, "scale_end", 0);

  out->drag = Json_GetFloat(j, "drag", 0);
  out->wid = Json_GetFloat(j, "width", 0);
  out->hei = Json_GetFloat(j, "height", 0);
  char start_col_str[MAX_NAME_LEN];
  Json_GetString(j, "start_color", start_col_str);
  out->start_color = ColorFromHexString(start_col_str);

  char end_col_str[MAX_NAME_LEN];
  Json_GetString(j, "end_color", end_col_str);
  out->end_color = ColorFromHexString(end_col_str);

  char draw_str[MAX_NAME_LEN];
  Json_GetString(j, "draw_type", draw_str);

  out->draw_type = StringToDrawType(draw_str);
  return out->layer > -1;
}

bool ParseSpriteComponent(cJSON* j, sprite_t* out){

  char sheet[MAX_NAME_LEN];
  Json_GetString(j, "sheet_id", sheet);
  int sheet_id = StringToSheetID(sheet);
  out->index = Json_GetInt(j, "sheet_index", -1);
  out->layer = Json_GetInt(j, "layer", -1);
  out->scale = Json_GetFloat(j, "scale", 1);
  out->sheet_id = sheet_id;

  return sheet_id > -1;
}


bool ParseAnimComponent(cJSON* j, anim_comp_t* out){
  char sheet[MAX_NAME_LEN];
  Json_GetString(j, "sheet_id", sheet);
  int sheet_id = StringToSheetID(sheet);

  cJSON* state_json = cJSON_GetObjectItem(j, "states");

  char* name = GameCalloc("ParseAnimComponent", MAX_NAME_LEN, sizeof(char));

  Json_GetString(j, "name", name);

  out->player.sheet_id = sheet_id;
  out->player.dir = 3;
  out->player.state = ANIM_IDLE;
  int sidx = 0;
  cJSON* s;
  cJSON_ArrayForEach(s, state_json){
    char* state_str = GameCalloc("ParseAnimComponent", MAX_NAME_LEN, sizeof(char));
    Json_GetString(s, "state", state_str);

    bool loop = Json_GetBool(s, "loop");
    bool interupt = Json_GetBool(s, "interupt");
    AnimState state = StringToAnimState(state_str);
    if(state == ANIM_NONE){
      TraceLog(LOG_WARNING,"=== PARSE ANIM COMP ===\n unable to find state %s for prefab %s", state_str, name);
    }
    cJSON* seq_json = cJSON_GetObjectItem(s, "sequences");
    cJSON* seq;
    cJSON_ArrayForEach(seq, seq_json){
      int dir = Json_GetInt(seq, "dir", -1);
      if(dir < 0)
        continue;

      char tag[MAX_NAME_LEN];
      Json_GetString(seq, "tag", tag);
      out->sequences[state][dir] = *AnimRegisterState(sheet_id, name, tag);
      out->sequences[state][dir].loop = loop;
      out->sequences[state][dir].interupt = interupt;
      cJSON* ev_json = cJSON_GetObjectItem(s, "events");
      if(!cJSON_IsArray(ev_json))
        continue;

      cJSON* on;
      cJSON_ArrayForEach(on, ev_json){
        char pname[MAX_NAME_LEN];
        Json_GetString(on, "phase", pname);
        AnimPhase p = AnimPhaseLookup(pname);

        char ename[MAX_NAME_LEN];
        Json_GetString(on, "event", ename);

        out->sequences[state][dir].on_phase[p] = StringToAnimEvent(ename);
      }
    }
  }

  sprite_sheet_d sh = SHEETS[sheet_id];
  int num_hurt = 0;
  for(int i = 0; i < MAX_SLICES; i++){
    collision_d cd = sh.coll[i];

    switch(cd.type){
      case COL_HIT:
        out->hitbox = cd;
        break;
      case COL_HURT:
        out->hurtboxes[num_hurt++] = cd;
      default:
        continue;
        break;
    }
  }
  return sheet_id > -1;
}

bool ParsePositionComponent(cJSON* j, position_t* out){
  if(!j)
    return false;

  float x = Json_GetFloat(j, "posx", 0.f);
  float y = Json_GetFloat(j, "posy", 0.f);

  Vector2 pos = VEC_NEW(x,y);
  out->last_pos = out->pos = pos;

  out->dir_step = out->dest = VEC_UNSET;
  out->rad = 0;
  out->angle = 0;
  return true;
}

bool ParseCameraComponent(cJSON* j, camera_t* out){
  if(!j)
    return false;

  out->zoom = Json_GetFloat(j, "zoom", 1.f);
  out->rotation = Json_GetFloat(j, "rotation", 0.f);

  float offx = Json_GetFloat(j, "offset_x", 0.f);
  float offy = Json_GetFloat(j, "offset_y", 0.f);

  float tarx = Json_GetFloat(j, "target_x", 0.f);
  float tary = Json_GetFloat(j, "target_y", 0.f);

  out->offset = VEC_NEW(offx, offy);
  out->target = VEC_NEW(tarx, tary);

  return true;
}
bool ParseRenderComponent(cJSON* j, render_ctx_t* out){
  if(!j)
    return false;

  Json_GetString(j, "name", out->name);
  out->layer = Json_GetInt(j, "layer", -1);
  return out->layer > -1;
}


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

  out->num_obs = l_num;
  
  return l_num > 0;
}

bool ParseSubjectComponent(cJSON* j, subject_component_t* out){
  if(!j)
    return false;

  Json_GetString(j, "name", out->name);

  char cname[MAX_NAME_LEN];
  Json_GetString(j, "comp", cname);
  out->comp = ComponentGetID(cname);

  out->type = Json_GetInt(j, "type", 0);
  out->event = EventIDLookup(out->name);
  return out->comp != INVALID_COMPONENT;

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

  out->restitution = 1;
  out->col_rate = Json_GetInt(j, "rate", 0);
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

  char name[MAX_NAME_LEN];
  Json_GetString(j, "name", name);
  if(out->speed == 0)
    TraceLog(LOG_WARNING, "=== PARSE FORCE ===\n Force %s speed is zero", name);
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

bool ParseBehaviorComponent(cJSON* j, behavior_t* out){
  if(!j)
    return false;

  Json_GetString(j, "name", out->name);

  char sname[MAX_NAME_LEN];
  Json_GetString(j, "state", sname);

  out->state = StringToState(sname);

  return out->state > STATE_NONE;
}

bool ParseTeamComponent(cJSON* j, team_t* out){
  if(!j)
    return false;

  Json_GetString(j, "team", out->name);
  out->id = hash_str_64(out->name);

  return out->id > 0;
}

bool ParseStatComponent(cJSON* j, stat_t* out){
  if(!j)
    return false;

  Json_GetString(j, "type", out->name);
  out->id = hash_str_64(out->name);
  out->current = Json_GetInt(j, "amount", 0);
  out->max = Json_GetInt(j, "max", out->current);
  out->min = Json_GetInt(j, "min", out->current);

  return out->current > 0 || out->max > 0;
}

bool ParseSubscribeComponent(cJSON* j, subscription_t* out){
  if(!j)
    return false;

  char ename[MAX_NAME_LEN];
  Json_GetString(j, "event", ename);
  out->event = EventIDLookup(ename);
  out->type = Json_GetInt(j, "type", 0);

  Json_GetString(j, "listener", out->listener);
  return out->type > 0;
}
