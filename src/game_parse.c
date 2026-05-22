#include "game_helpers.h"
#include "game_strings.h"
#include "system_define.h"
#include "scene.h"

char* Json_GetString(cJSON* obj, const char* key, char* out)
{
  size_t max_len = MAX_NAME_LEN -1;

  cJSON* item = cJSON_GetObjectItem(obj, key);
  if (item && cJSON_IsString(item) && out) {
    strncpy(out, item->valuestring, max_len);
    out[max_len] = '\0';
    return out;
  }
  out[0] = '\0';
  return out;
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

AnimBehavior StringToAnimBehavior(char* str){
 if (strcmp(str, "BLANK") == 0) return ANIM_BLANK;
 if (strcmp(str, "SUSPEND") == 0) return ANIM_SUSPEND;
 if (strcmp(str, "HURTBOX") == 0) return ANIM_HURTBOX;

 return ANIM_BLANK;
}

AnimState StringToAnimState(char* str){
 if (strcmp(str, "IDLE") == 0) return ANIM_IDLE;
 if (strcmp(str, "WALK") == 0) return ANIM_WALK;
 if (strcmp(str, "ATTACK") == 0) return ANIM_ATTACK;
 if (strcmp(str, "DIE") == 0) return ANIM_DIE;
 if (strcmp(str, "HURT") == 0) return ANIM_HURT;

 return ANIM_NONE;
}

SheetID StringToSheetID(const char* str){
  if (strcmp(str, "SHEET_CHAR") == 0) return SHEET_CHAR;
  if (strcmp(str, "SHEET_PLAYER") == 0) return SHEET_CHAR;
  if (strcmp(str, "SHEET_MOB") == 0)  return SHEET_MOB;
  if (strcmp(str, "SHEET_TILE") == 0) return SHEET_TILE;
  // ... add others
  return SHEET_ALL;
}

ParticleEmitMode StringToEmitMode(const char* str){
  if(strcmp(str, "CONTINUOUS") == 0) return PARTICLE_EMIT_CONTINUOUS;
  if(strcmp(str, "BURST") == 0) return PARTICLE_EMIT_BURST;
  if(strcmp(str, "EVENT") == 0) return PARTICLE_EMIT_EVENT;

  return PARTICLE_EMIT_NONE;
}

ParticleDrawType StringToDrawType(const char* str){
  if(strcmp(str, "SPRITE") == 0) return PARTICLE_SPRITE;
  if(strcmp(str, "RECT") == 0) return PARTICLE_RECT;
  if(strcmp(str, "CIRCLE") == 0) return PARTICLE_CIRCLE;
  if(strcmp(str, "PIXEL") == 0) return PARTICLE_PIXEL;

  return PARTICLE_NONE;
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
    char end_str[MAX_NAME_LEN];
    Json_GetString(s, "end", end_str);
    AnimBehavior on_end = StringToAnimBehavior(end_str); 

    char start_str[MAX_NAME_LEN];
    Json_GetString(s, "start", start_str);
    AnimBehavior on_start = StringToAnimBehavior(start_str);

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
      out->sequences[state][dir].on_end = on_end;
    }  
  }

  sprite_sheet_d sh = SHEETS[sheet_id];
  for(int i = 0; i < MAX_SLICES; i++){
    collision_d cd = sh.coll[i];

    switch(cd.type){
      case COL_HIT:
        out->hitbox = cd;
        break;
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
  out->last_vpos = out->vpos = pos;

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

RelationType RelationTypeLookup(char* str){
  for(int i = 0; i < NUM_REL; i++){
    if (strcmp(str, RELATION_LOOKUP[i].name) == 0)
      return RELATION_LOOKUP[i].type;
  }
}

ComponentInitFn ComponentFuncLookup(const char* name){
  if (!name) return NULL;

  for (int i = 0; i < NUM_COMP_CORE; i++)   // your function registry
  { 
    if (strcmp(COMPFUNC_LOOKUP[i].name, name) == 0)
      return COMPFUNC_LOOKUP[i].func;   // cast if needed
  }

  TraceLog(LOG_WARNING,"=== COMPONENT FUNC LOOKUP ===\n function '%s' not registered!\n", name);
  return NULL;

}

SystemFn SystemFunctionLookup(const char* name)
{
  if (!name) return NULL;

  for (int i = 0; i < NUM_FUNCTIONS; i++)   // your function registry
  {
    if (strcmp(FUNCTION_LOOKUP[i].name, name) == 0)
      return (SystemFn)FUNCTION_LOOKUP[i].func;   // cast if needed
  }

  TraceLog(LOG_WARNING,"=== SYSTEM LOOKUP ===\n system function '%s' not registered!\n", name);
  return NULL;
}

GameState GetGameState(const char* name){
  if(!name) return -1;

  for(int i = 0; i < GAME_DONE; i++){
    if (strcmp(GAMESTATE_LOOKUP[i].name, name) == 0)
      return GAMESTATE_LOOKUP[i].state;
  }

  TraceLog(LOG_WARNING,"==== GAMESTATE LOOKUP ====\n %s not found!", name);

  return -1;
} 

UpdateType GetUpdateStep(const char* name){
  if(!name) return -1;

  for(int i = 0; i < UPDATE_DONE; i++){
    if (strcmp(UPDATE_LOOKUP[i].name, name) == 0)
      return UPDATE_LOOKUP[i].type;
  }

  TraceLog(LOG_WARNING,"==== UPDATE LOOKUP ====\n %s not found!", name);

  return -1;
}


cJSON* ParseRoot(const char* path){
  char* json_str = LoadFileText(path);   // Raylib helper
  if (!json_str) {
    TraceLog(LOG_ERROR, "Failed to load %s", path);
    return NULL;
  }

  cJSON* root = cJSON_Parse(json_str);
  UnloadFileText(json_str);
  if (!root) {
    TraceLog(LOG_ERROR, "JSON parse error: %s", path);
    return NULL;
  }

  return root;
}

bool LoadAsepriteSheet(cJSON* root, ase_sprite_sheet_d* sheet){
  if (!sheet) return false;
  cJSON* meta = cJSON_GetObjectItem(root, "meta");

  // ====================== PARSE TAGS (frameTags) ======================
  cJSON* tags_json = cJSON_GetObjectItem(meta, "frameTags");
  if (tags_json) {
    int count = cJSON_GetArraySize(tags_json);
    sheet->num_tags = count;   // assuming you have tag_count in ase_sprite_sheet_d

    for (int i = 0; i < count && i < MAX_ANIM_GROUPS; i++) {
      cJSON* t = cJSON_GetArrayItem(tags_json, i);
      anim_tag_t* tag = &sheet->tags[i];
      const char* tagname = cJSON_GetObjectItem(t, "name")->valuestring;
      strcpy(tag->name, tagname);

      int count;
      char** parts = split_string(tag->name, '_', &count);

      tag->hash = hash_str_32(parts[0]);

      char* group_str = str_concat(parts[1], parts[2]);

      tag->group = hash_str_64(group_str);
      // Range
      tag->index_start = cJSON_GetObjectItem(t, "from")->valueint;
      tag->index_end   = cJSON_GetObjectItem(t, "to")->valueint;

      // Repeat
      cJSON* repeat_item = cJSON_GetObjectItem(t, "repeat");
      tag->repeat = repeat_item ? true : false;

      // Direction
      cJSON* dir_item = cJSON_GetObjectItem(t, "direction");
      if (dir_item && dir_item->valuestring) {
        strncpy(tag->direction, dir_item->valuestring, 15);
        tag->direction[15] = '\0';
      } else {
        strcpy(tag->direction, "forward");
      }
    }
  }
  // ====================== PARSE FRAMES ======================
  cJSON* frames_json = cJSON_GetObjectItem(root, "frames");
  sheet->num_frames = cJSON_GetArraySize(frames_json);
  sheet->frame_meta = cJSON_Duplicate(frames_json, true);
  
  for (int i = 0; i < sheet->num_frames && i < MAX_SPRITE_FRAMES; i++) {
    cJSON* f = cJSON_GetArrayItem(frames_json, i);
    anim_frame_t* frame = &sheet->frames[i];

    // Parse filename → tag + group
    frame->index = i;

    frame->duration = cJSON_GetObjectItem(f, "duration")->valueint;
    // Frame rect
    cJSON* fr = cJSON_GetObjectItem(f, "frame");
    frame->frame_rect.x      = (float)cJSON_GetObjectItem(fr, "x")->valuedouble;
    frame->frame_rect.y      = (float)cJSON_GetObjectItem(fr, "y")->valuedouble;
    frame->frame_rect.width  = (float)cJSON_GetObjectItem(fr, "w")->valuedouble;
    frame->frame_rect.height = (float)cJSON_GetObjectItem(fr, "h")->valuedouble;

    // Source rect
    cJSON* src = cJSON_GetObjectItem(f, "spriteSourceSize");
    frame->source_rect.x      = (float)cJSON_GetObjectItem(src, "x")->valuedouble;
    frame->source_rect.y      = (float)cJSON_GetObjectItem(src, "y")->valuedouble;
    frame->source_rect.width  = (float)cJSON_GetObjectItem(src, "w")->valuedouble;
    frame->source_rect.height = (float)cJSON_GetObjectItem(src, "h")->valuedouble;

  }
  // ====================== PARSE SLICES ======================
  cJSON* slices_json = cJSON_GetObjectItem(meta, "slices");
  sheet->num_slices = cJSON_GetArraySize(slices_json);

  sheet->slice_meta = cJSON_Duplicate(slices_json, true); 
  for (int i = 0; i < sheet->num_slices && i < MAX_ANIM_GROUPS; i++) {
    cJSON* s = cJSON_GetArrayItem(slices_json, i);
    slice_d* slice = &sheet->slices[i];

    const char* name = cJSON_GetObjectItem(s, "name")->valuestring;
    strncpy(slice->name, name, MAX_NAME_LEN-1);

    cJSON* keys_json = cJSON_GetObjectItem(s, "keys");

    int k_size = cJSON_GetArraySize(keys_json);

    int k_count = 0;
    for (int k = 0; k < k_size && k < MAX_SLICES; k++) {
      cJSON* key = cJSON_GetArrayItem(keys_json, k);
      cJSON* b = cJSON_GetObjectItem(key, "bounds");

      int k_frame = cJSON_GetObjectItem(key, "frame")->valueint;
      slice->keys[k_count].frame = k_frame;
      slice->keys[k_count].bounds.x      = (float)cJSON_GetObjectItem(b, "x")->valuedouble;
      slice->keys[k_count].bounds.y      = (float)cJSON_GetObjectItem(b, "y")->valuedouble;
      slice->keys[k_count].bounds.width  = (float)cJSON_GetObjectItem(b, "w")->valuedouble;
      slice->keys[k_count].bounds.height = (float)cJSON_GetObjectItem(b, "h")->valuedouble;
      k_count++;
    }

    slice->num_keys = k_count;
  }
  return true;
}
// Cleanup
void Unloadsprite_sheet_d(sprite_sheet_d* sheet)
{
}

bool ParseScene(cJSON* root, Scene* scene){
  Json_GetString(root, "display_name", scene->display_name);

  scene->pixel_width  = Json_GetInt(root, "pixel_width", 800);
  scene->pixel_height = Json_GetInt(root, "pixel_height", 600);
  scene->grid_width   = Json_GetInt(root, "grid_width", 18);
  scene->grid_height  = Json_GetInt(root, "grid_height", 11);
  scene->cell_width   = Json_GetInt(root, "cell_width", 80);
  scene->cell_height  = Json_GetInt(root, "cell_height", 80);

  // Parse Tiles
  cJSON* tile_array = cJSON_GetObjectItem(root, "tiles");
  if (tile_array && cJSON_IsArray(tile_array)) {
    scene->tile_count = cJSON_GetArraySize(tile_array);
    scene->tiles = GameCalloc("ParseScene", scene->tile_count, sizeof(tile_instance_t));

    for (int i = 0; i < scene->tile_count; i++) {
      cJSON* item = cJSON_GetArrayItem(tile_array, i);
      tile_instance_t* t = &scene->tiles[i];
      t->name = GameCalloc("ParseScene", MAX_NAME_LEN, sizeof(char));
      Json_GetString(item, "name", t->name);
      t->cell_x      = Json_GetInt(item, "cell_x", 0);
      t->cell_y      = Json_GetInt(item, "cell_y", 0);
      t->rotation   = Json_GetInt(item, "rotation", 0);
      t->flip_x     = cJSON_IsTrue(cJSON_GetObjectItem(item, "flip_x"));
      t->flip_y     = cJSON_IsTrue(cJSON_GetObjectItem(item, "flip_y"));
    }
  }

  // Parse Entities
  cJSON* ent_array = cJSON_GetObjectItem(root, "entities");
  if (ent_array && cJSON_IsArray(ent_array)) {
    scene->entity_count = cJSON_GetArraySize(ent_array);
    scene->entities = GameCalloc("ParseScene", scene->entity_count, sizeof(entity_instance_t));

    for (int i = 0; i < scene->entity_count; i++) {
      cJSON* item = cJSON_GetArrayItem(ent_array, i);
      entity_instance_t* e = &scene->entities[i];
      e->prefab = GameCalloc("ParseScene", MAX_NAME_LEN, sizeof(char));

      Json_GetString(item, "name", e->prefab);
      e->x = Json_GetFloat(item, "pos_x", 0.0f);
      e->y = Json_GetFloat(item, "pos_y", 0.0f);
      e->type = Json_GetInt(item, "type", 0);
    }
  }

  // Metadata
  cJSON* meta = cJSON_GetObjectItem(root, "metadata");
  if (meta)
    scene->metadata = meta;

  cJSON_Delete(root);
  return true;


}

bool ParseSystems(cJSON* root, game_t* out){
  if (!root || !out) return false;

  cJSON* systems_json = cJSON_GetObjectItem(root,"systems");

  out->num_sys = cJSON_GetArraySize(systems_json);
  if (out->num_sys > NUM_SYS)
  {
    printf("WARNING: Too many systems (%d > %d). Truncating.\n", out->num_sys, NUM_SYS);
    out->num_sys = NUM_SYS;
  }

  int sys_idx = 0;
  cJSON* sys_name;
  cJSON_ArrayForEach(sys_name, systems_json)
  {
    cJSON* sys_item = cJSON_GetObjectItem(root, sys_name->valuestring);
    if (sys_idx >= NUM_SYS) break;

    system_define_t* sys = &out->systems[sys_idx++];

    // --- Name ---
    sys->name = GameCalloc("ParseSystems", MAX_NAME_LEN, sizeof(char));
    strcpy(sys->name, sys_name->valuestring);

    // --- Components ---
    cJSON* comps_json = cJSON_GetObjectItem(sys_item, "components");
    if (cJSON_IsArray(comps_json))
    {
      int cidx = 0;
      cJSON* c;
      cJSON_ArrayForEach(c, comps_json)
      {
        if (!cJSON_IsString(c) || cidx >= NUM_REL)
          continue;

          sys->components[cidx] = GameCalloc("ParseSystems", MAX_NAME_LEN, sizeof(char));
          sys->components[cidx++] = c->valuestring;
      }
      sys->num_req = cidx;
    }

    
    cJSON* state_json = cJSON_GetObjectItem(sys_item, "states");
    if (cJSON_IsArray(state_json)){
      cJSON* state;
      cJSON_ArrayForEach(state, state_json){
        cJSON* step_json = cJSON_GetObjectItem(state, "step");
        cJSON* fn_json   = cJSON_GetObjectItem(state, "fn");
        if (!cJSON_IsString(step_json) || !cJSON_IsString(fn_json))
          continue;

        SystemFn callback = (SystemFn)SystemFunctionLookup(fn_json->valuestring);
        if (!callback) continue;

        GameState gs = GetGameState(step_json->valuestring);
        if (gs >= 0 && gs < GAME_DONE)
          sys->states[gs] = callback;

      }
    } 
    
    cJSON* sync_json = cJSON_GetObjectItem(sys_item, "syncs");
    if (cJSON_IsArray(sync_json))
    {
      cJSON* sync;
      cJSON_ArrayForEach(sync, sync_json)
      {
        cJSON* step_json = cJSON_GetObjectItem(sync, "step");
        cJSON* fn_json   = cJSON_GetObjectItem(sync, "fn");

        if (!cJSON_IsString(step_json) || !cJSON_IsString(fn_json))
          continue;

        SystemCB callback = (SystemCB)SystemFunctionLookup(fn_json->valuestring);
        if (!callback) continue;

        UpdateType step = GetUpdateStep(step_json->valuestring);
        if (step < 0 || step >= UPDATE_DONE)
          continue;

          sys->syncs[step] = callback;
      }
    }
    cJSON* set_json = cJSON_GetObjectItem(sys_item, "sets");
    if (cJSON_IsArray(set_json))
    {
      cJSON* set;
      cJSON_ArrayForEach(set, set_json)
      {
        cJSON* step_json = cJSON_GetObjectItem(set, "step");
        cJSON* fn_json   = cJSON_GetObjectItem(set, "fn");

        if (!cJSON_IsString(step_json) || !cJSON_IsString(fn_json))
          continue;

        SystemCB callback = (SystemCB)SystemFunctionLookup(fn_json->valuestring);
        if (!callback) continue;

        GameState gs = GetGameState(step_json->valuestring);
        if (gs >= 0 && gs < GAME_DONE)
          sys->sets[gs] = callback;
      }
    }

    cJSON* steps_json = cJSON_GetObjectItem(sys_item, "steps");
    if (cJSON_IsArray(steps_json))
    {
      cJSON* steps;
      cJSON_ArrayForEach(steps, steps_json)
      {
        cJSON* step_json = cJSON_GetObjectItem(steps, "step");
        cJSON* fn_json   = cJSON_GetObjectItem(steps, "fn");

        if (!cJSON_IsString(step_json) || !cJSON_IsString(fn_json))
          continue;

        SystemFn callback = (SystemFn)SystemFunctionLookup(fn_json->valuestring);
        if (!callback) continue;

        UpdateType step = GetUpdateStep(step_json->valuestring);
        if (step < 0 || step >= UPDATE_DONE)
          continue;

        sys->steps[step] = callback;
      }
    }
  
    sys->iter = Json_GetBool(sys_item, "iter");


  }

  return true;
}

bool ParseComponents(cJSON* root, game_t* out){
  cJSON* comp_json = cJSON_GetObjectItem(root, "components");
  if(cJSON_IsArray(comp_json)){
    int idx = 0;
    int comps_present = cJSON_GetArraySize(comp_json);

    cJSON* c;
    cJSON_ArrayForEach(c, comp_json){
      if (!cJSON_IsString(c))
        continue;

      out->comps[out->num_comps++] = c->valuestring;

      cJSON* comp_obj = cJSON_GetObjectItem(root, c->valuestring);

      if(!comp_obj){
        TraceLog(LOG_WARNING, "==== PARSE COMPONENTS ===\n %s missing definition!", c->valuestring);
        continue;
      }

      cJSON* list  = cJSON_GetObjectItem(comp_obj, "list");

      cJSON* entry;
      cJSON_ArrayForEach(entry, list){
        cJSON* contain_list = cJSON_GetObjectItem(entry, "contains");
        if(!contain_list){
          component_entry_t* sc = &out->comp_defs[idx++];
          sc->name = GameCalloc("ParseComponents", MAX_NAME_LEN, sizeof(char));
          sc->comp = GameCalloc("ParseComponents", MAX_NAME_LEN, sizeof(char));

          Json_GetString(entry, "name", sc->name);
          sc->func = ComponentFuncLookup(c->valuestring);
          sc->data = entry;
          sc->comp = c->valuestring;
        }
        else{
          cJSON* sub;
          cJSON_ArrayForEach(sub, contain_list){
            component_entry_t* comp = &out->comp_defs[idx++];
            comp->name = GameCalloc("ParseComponents", MAX_NAME_LEN, sizeof(char));
            comp->comp = GameCalloc("ParseComponents", MAX_NAME_LEN, sizeof(char));

            strcpy(comp->name, sub->valuestring);

            comp->comp = c->valuestring;
            comp->func = ComponentFuncLookup(c->valuestring);
            comp->data = entry;
          }
        }
      }
    }
    return true;
  }

  return false;
}

void ParsePrefab(cJSON* item, prefab_entity_t* out){
  cJSON* comp_json = cJSON_GetObjectItem(item, "list");
  if (cJSON_IsArray(comp_json))
  {
    int comp_idx = 0;
    cJSON* comp;
    cJSON_ArrayForEach(comp, comp_json)
    {
      if (cJSON_IsString(comp) && comp_idx < MAX_COMPONENTS)
      {
        size_t len = strlen(comp->valuestring) + 1;
        out->components[comp_idx] = GameMalloc("ParsePrefabs", len);
        strcpy((char*)out->components[comp_idx], comp->valuestring);
        comp_idx++;
      }
    }
    out->num_comp = comp_idx;
  }

}

bool ParsePrefabs(cJSON* root, game_t* out){
  cJSON* prefabs_json = cJSON_GetObjectItem(root, "prefabs");
  cJSON* list_json = cJSON_GetObjectItem(prefabs_json, "list");
  if (!cJSON_IsArray(list_json))
  {
    TraceLog(LOG_ERROR, "ParseGameDef: 'prefabs.list' is not an array");
    return false;
  }


  int i = 0;
  cJSON* item;
  cJSON_ArrayForEach(item, list_json)
  {
    prefab_entity_t* p;// = &out->prefabs[i++];

    cJSON* contains_json = cJSON_GetObjectItem(item, "contains");
    if (cJSON_IsArray(contains_json))
    {
      cJSON* variant;
      cJSON_ArrayForEach(variant, contains_json){
        p = &out->prefabs[i++];
        p->name = GameCalloc("ParsePrefabs", MAX_NAME_LEN, sizeof(char));
        strcpy(p->name, variant->valuestring);
        ParsePrefab(item, p);
      }
    }
    else{
      p = &out->prefabs[i++];
      p->name = GameCalloc("ParsePrefabs", MAX_NAME_LEN, sizeof(char));

      Json_GetString(item, "name", p->name);
      ParsePrefab(item, p);
    }
  }
  out->num_prefabs = i;
  return true;

}

bool ParseRelations(cJSON* root, game_t* out){
  cJSON* relations_json = cJSON_GetObjectItem(root, "relations");
  if (cJSON_IsArray(relations_json))
  {
    out->relation_count = cJSON_GetArraySize(relations_json);
    out->relations = GameCalloc("ParseGameDef", out->relation_count, sizeof(entity_relation_t));
    if (!out->relations) return false;

    int rel_idx = 0;
    cJSON* rel_item;
    cJSON_ArrayForEach(rel_item, relations_json)
    {
      entity_relation_t* er = &out->relations[rel_idx++];
      er->name = GameCalloc("ParseRelations", MAX_NAME_LEN, sizeof(char));
      Json_GetString(rel_item, "name", er->name);


      cJSON* list = cJSON_GetObjectItem(rel_item, "list");
      if (!cJSON_IsArray(list)) continue;
      er->count = cJSON_GetArraySize(list);

      int comp_rel_idx = 0;
      cJSON* comp_rel;
      cJSON_ArrayForEach(comp_rel, list)
      {
        if (comp_rel_idx >= MAX_RELATIONS) break;

        component_relation_t* cr = &er->comps[comp_rel_idx++];
        cr->comp = GameCalloc("ParseRelations", MAX_NAME_LEN, sizeof(char));

        Json_GetString(comp_rel, "name", cr->comp);
        cJSON* sublist = cJSON_GetObjectItem(comp_rel, "sublist");

        if (cJSON_IsArray(sublist))
        {
          int pair_idx = 0;
          cJSON* pair;
          cJSON_ArrayForEach(pair, sublist)
          {
            if (pair_idx >= MAX_RELATIONS_PER_ENTITY) break;

            cr->pairs[pair_idx].name = GameCalloc("ParseRelations", MAX_NAME_LEN, sizeof(char));

            Json_GetString(pair, "name", cr->pairs[pair_idx].name);
            cJSON* ptype = cJSON_GetObjectItem(pair, "type");

            if (cJSON_IsString(ptype))
              cr->pairs[pair_idx].type = RelationTypeLookup(ptype->valuestring);
            
            cJSON* relcomp_json = cJSON_GetObjectItem(pair, "components");
            
            int relidx = 0;
            cJSON* relcomp;
            cJSON_ArrayForEach(relcomp, relcomp_json){
              cr->pairs[pair_idx].components[relidx] = GameCalloc("ParseRelations", MAX_NAME_LEN, sizeof(char));

            strcpy(cr->pairs[pair_idx].components[relidx++], relcomp->valuestring);
            }
            cr->pairs[pair_idx].num_comp = relidx;
            pair_idx++;
          }
          cr->count = pair_idx;
        }
      }
    }

    return true;
  }

  return false;
}
