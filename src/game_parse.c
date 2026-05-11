#include "game_assets.h"
#include "game_define.h"
#include "game_strings.h"
<<<<<<< HEAD
#include "game_systems.h"
#include "scene.h"

char* Json_GetString(cJSON* obj, const char* key, char* out)
{
  size_t max_len = MAX_NAME_LEN -1;

  cJSON* item = cJSON_GetObjectItem(obj, key);
  if (item && cJSON_IsString(item) && out) {
    strncpy(out, item->valuestring, max_len);
    out[max_len] = '\0';
=======
#include "scene.h"

static char* Json_GetString(cJSON* obj, const char* key, char* out, int max_len)
{
  cJSON* item = cJSON_GetObjectItem(obj, key);
  if (item && cJSON_IsString(item) && out) {
    strncpy(out, item->valuestring, max_len - 1);
    out[max_len-1] = '\0';
>>>>>>> 9a78f7bda89d2d6c55d3d73a5974c422944a4594
    return out;
  }
  out[0] = '\0';
  return out;
}

<<<<<<< HEAD
int Json_GetInt(cJSON* obj, const char* key, int default_val)
=======
static int Json_GetInt(cJSON* obj, const char* key, int default_val)
>>>>>>> 9a78f7bda89d2d6c55d3d73a5974c422944a4594
{
  cJSON* item = cJSON_GetObjectItem(obj, key);
  return cJSON_IsNumber(item) ? item->valueint : default_val;
}

<<<<<<< HEAD
float Json_GetFloat(cJSON* obj, const char* key, float default_val)
=======
static float Json_GetFloat(cJSON* obj, const char* key, float default_val)
>>>>>>> 9a78f7bda89d2d6c55d3d73a5974c422944a4594
{
  cJSON* item = cJSON_GetObjectItem(obj, key);
  return cJSON_IsNumber(item) ? (float)item->valuedouble : default_val;
}

<<<<<<< HEAD
bool ParseSpriteComponent(cJSON* j, sprite_t* out){

  int sheet = Json_GetInt(j, "sheet_id", -1);
  int idx = Json_GetInt(j, "sheet_index", -1);

  out->sheet_id = sheet;
  out->index = idx;
}

bool ParseRigidBodyComponent(cJSON* j, rigid_body_t* out){

}

bool ParseForceComponent(cJSON* j, force_t* out){

}

bool ParseAnimComponent(cJSON* j, anim_comp_t* out){

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

bool ParseCameraComponent(cJSON* j, cam_comp_t* out){
  if(!j)
    return false;

  float zoom = Json_GetFloat(j, "zoom", 1.f);
  float rot = Json_GetFloat(j, "rotation", 0.f);

  float offx = Json_GetFloat(j, "offset_x", 0.f);
  float offy = Json_GetFloat(j, "offset_y", 0.f);

  Vector2 offset = VEC_NEW(offx, offy);

  out->camera = *InitCamera(zoom, rot, offset);

  float bx = Json_GetFloat(j, "bounds_x", 0.f);
  float by = Json_GetFloat(j, "bounds_y", 0.f);
  float bw = Json_GetFloat(j, "bounds_w", 0.f);
  float bh = Json_GetFloat(j, "bounds_h", 0.f);
 
  Rectangle bounds = RECT(bx, by, bw, bh);
  Vector2 size = VEC_NEW(bw, bh); 

  out->view = *InitView(size, bounds, 0);

  return true;
}


=======
>>>>>>> 9a78f7bda89d2d6c55d3d73a5974c422944a4594
RelationType RelationTypeLookup(char* str){
  for(int i = 0; i < NUM_REL; i++){
    if (strcmp(str, RELATION_LOOKUP[i].name) == 0)
      return RELATION_LOOKUP[i].type;
  }
}
<<<<<<< HEAD

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
=======
>>>>>>> 9a78f7bda89d2d6c55d3d73a5974c422944a4594

SheetID StringToSheetID(const char* str)
{
  if (strcmp(str, "SHEET_CHAR") == 0) return SHEET_CHAR;
  if (strcmp(str, "SHEET_MOB") == 0)  return SHEET_MOB;
  if (strcmp(str, "SHEET_TILE") == 0) return SHEET_TILE;
  // ... add others
  return SHEET_ALL;
}

cJSON* ParseRoot(const char* path){
  char* json_str = LoadFileText(path);   // Raylib helper
  if (!json_str) {
    TraceLog(LOG_ERROR, "Failed to load %s", path);
    return NULL;
  }

  cJSON* root = cJSON_Parse(json_str);
  if (!root) {
    TraceLog(LOG_ERROR, "JSON parse error: %s", path);
    UnloadFileText(json_str);
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

  for (int i = 0; i < sheet->num_slices && i < MAX_ANIM_FRAMES; i++) {
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


      CollType ct = COL_HURT;
      if(name){
        if(strcmp(name, "hitbox") == 0)
          ct = COL_HIT;
      }

      int k_frame = cJSON_GetObjectItem(key, "frame")->valueint;
      slice->keys[k_count].type = ct;
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
<<<<<<< HEAD
  Json_GetString(root, "display_name", scene->display_name);
=======
  Json_GetString(root, "display_name", scene->display_name, 128);
>>>>>>> 9a78f7bda89d2d6c55d3d73a5974c422944a4594

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
<<<<<<< HEAD
    scene->tiles = GameCalloc("ParseScene", scene->tile_count, sizeof(tile_instance_t));

    for (int i = 0; i < scene->tile_count; i++) {
      cJSON* item = cJSON_GetArrayItem(tile_array, i);
      tile_instance_t* t = &scene->tiles[i];
      t->name = GameCalloc("ParseScene", MAX_NAME_LEN, sizeof(char));
      Json_GetString(item, "name", t->name);
=======
    scene->tiles = GameCalloc("ParseScene", scene->tile_count, sizeof(TileInstance));

    for (int i = 0; i < scene->tile_count; i++) {
      cJSON* item = cJSON_GetArrayItem(tile_array, i);
      TileInstance* t = &scene->tiles[i];
      t->tile_index = Json_GetInt(item, "tile_index", 0);
>>>>>>> 9a78f7bda89d2d6c55d3d73a5974c422944a4594
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
<<<<<<< HEAD
    scene->entities = GameCalloc("ParseScene", scene->entity_count, sizeof(entity_instance_t));

    for (int i = 0; i < scene->entity_count; i++) {
      cJSON* item = cJSON_GetArrayItem(ent_array, i);
      entity_instance_t* e = &scene->entities[i];
      e->prefab = GameCalloc("ParseScene", MAX_NAME_LEN, sizeof(char));

      Json_GetString(item, "name", e->prefab);
=======
    scene->entities = GameCalloc("ParseScene", scene->entity_count, sizeof(EntityInstance));

    for (int i = 0; i < scene->entity_count; i++) {
      cJSON* item = cJSON_GetArrayItem(ent_array, i);
      EntityInstance* e = &scene->entities[i];
      Json_GetString(item, "prefab", e->prefab, 64);
>>>>>>> 9a78f7bda89d2d6c55d3d73a5974c422944a4594
      e->x = Json_GetFloat(item, "x", 0.0f);
      e->y = Json_GetFloat(item, "y", 0.0f);
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

<<<<<<< HEAD
bool ParseSystems(cJSON* root, game_t* out)
{
  if (!root || !out) return false;

  cJSON* systems_json = cJSON_GetObjectItem(root, "systems");
  if (!cJSON_IsArray(systems_json))
  {
    printf("ERROR: 'systems' array not found or invalid.\n");
    return false;
  }

  out->num_sys = cJSON_GetArraySize(systems_json);
  if (out->num_sys > NUM_SYS)
  {
    printf("WARNING: Too many systems (%d > %d). Truncating.\n", out->num_sys, NUM_SYS);
    out->num_sys = NUM_SYS;
  }

  int sys_idx = 0;
  cJSON* sys_item;
  cJSON_ArrayForEach(sys_item, systems_json)
  {
    if (sys_idx >= NUM_SYS) break;

    system_define_t* sys = &out->systems[sys_idx++];

    // --- Name ---
    sys->name = GameCalloc("ParseSystems", MAX_NAME_LEN, sizeof(char));
    Json_GetString(sys_item, "name", sys->name);

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
    }

    // --- Init function (optional) ---
    cJSON* init_json = cJSON_GetObjectItem(sys_item, "init");
    if (cJSON_IsString(init_json))
      sys->init = SystemFunctionLookup(init_json->valuestring);

    // --- Functions array ---
    cJSON* funcs_json = cJSON_GetObjectItem(sys_item, "functions");
    if (cJSON_IsArray(funcs_json))
    {
      cJSON* f;
      cJSON_ArrayForEach(f, funcs_json)
      {
        cJSON* step_json = cJSON_GetObjectItem(f, "step");
        cJSON* fn_json   = cJSON_GetObjectItem(f, "fn");

        if (!cJSON_IsString(step_json) || !cJSON_IsString(fn_json))
          continue;

        SystemCB callback = (SystemCB)SystemFunctionLookup(fn_json->valuestring);
        if (!callback) continue;

        if(step_json->valuestring[0] == 'U'){
        UpdateType step = GetUpdateStep(step_json->valuestring);
        if (step < 0 || step >= UPDATE_DONE)
          continue;

          sys->steps[step] = callback;
        }
        else
        {
          GameState state = GetGameState(step_json->valuestring);
          if (state >= 0 && state < GAME_DONE)
            sys->states[state] = callback;
        }
      }
    }
  }

  TraceLog(LOG_INFO,"===== GAME PARSE ====\nSuccessfully parsed %d systems.\n", out->num_sys);
  return true;
}

bool ParseComponents(cJSON* root, game_t* out){
  cJSON* comp_json = cJSON_GetObjectItem(root, "components");
  if(cJSON_IsArray(comp_json)){
    int idx = 0;
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
        component_entry_t* comp = &out->comp_defs[idx++];
        comp->name = GameCalloc("ParseComponents", MAX_NAME_LEN, sizeof(char));
        Json_GetString(entry, "name", comp->name);
        comp->func = ComponentFuncLookup(c->valuestring);
        comp->data = entry;
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


  out->num_prefabs = Json_GetInt(prefabs_json, "count", 0);
  if (out->num_prefabs == 0)
  {
    TraceLog(LOG_WARNING, "ParseGameDef: No prefabs defined");
    return false;
  }

  out->prefabs = GameCalloc("ParsePrefabs", out->num_prefabs, sizeof(prefab_entity_t));

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
  return true;

}

bool ParseRelations(cJSON* root, game_t* out){
=======
bool ParseGameDefinition(cJSON* root, game_t* out){
  if (!root || !out) return false;
  memset(out, 0, sizeof(*out));

  // ==================== COMPONENTS======================
  cJSON* comp_json = cJSON_GetObjectItem(root, "components");
  if(cJSON_IsArray(comp_json)){
    
    cJSON* c;
    cJSON_ArrayForEach(c, comp_json){
       if (cJSON_IsString(c))
         out->comps[out->num_comps++] = c->valuestring;
    }

  }

  // ====================== PREFABS ======================
  cJSON* prefabs_json = cJSON_GetObjectItem(root, "prefabs");
  if (cJSON_IsArray(prefabs_json))
  {
    out->num_prefabs = cJSON_GetArraySize(prefabs_json);
    out->prefabs = GameCalloc("ParseGameDef", out->num_prefabs, sizeof(prefab_entity_t));
    if (!out->prefabs) return false;

    int i = 0;
    cJSON* item;
    cJSON_ArrayForEach(item, prefabs_json)
    {
      prefab_entity_t* p = &out->prefabs[i++];

      cJSON* name  = cJSON_GetObjectItem(item, "name");
      cJSON* count = cJSON_GetObjectItem(item, "count");
      cJSON* list  = cJSON_GetObjectItem(item, "list");

      if (cJSON_IsString(name))
        p->name = strdup(name->valuestring);

      if (cJSON_IsNumber(count))
        p->num_comp = count->valueint;   // note: "count" in JSON is instance count, not component count

      if (cJSON_IsArray(list))
      {
        int comp_idx = 0;
        cJSON* comp;
        cJSON_ArrayForEach(comp, list)
        {
          if (cJSON_IsString(comp) && comp_idx < MAX_COMPONENTS)
          {
            p->components[comp_idx++] = strdup(comp->valuestring);
          }
        }
        p->num_comp = comp_idx;   // actual number of components in the list
      }
    }
  }

  // ====================== RELATIONS ======================
>>>>>>> 9a78f7bda89d2d6c55d3d73a5974c422944a4594
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
<<<<<<< HEAD
      er->name = GameCalloc("ParseRelations", MAX_NAME_LEN, sizeof(char));
      Json_GetString(rel_item, "name", er->name);
=======

      // The key is the entity name (e.g. "player")
      cJSON* rname  = cJSON_GetObjectItem(rel_item, "name");
      er->name = strdup(rname->valuestring);

>>>>>>> 9a78f7bda89d2d6c55d3d73a5974c422944a4594

      cJSON* count = cJSON_GetObjectItem(rel_item, "count");
      if (cJSON_IsNumber(count))
        er->count = count->valueint;

      cJSON* list = cJSON_GetObjectItem(rel_item, "list");
      if (!cJSON_IsArray(list)) continue;

      int comp_rel_idx = 0;
      cJSON* comp_rel;
      cJSON_ArrayForEach(comp_rel, list)
      {
        if (comp_rel_idx >= MAX_RELATIONS) break;

        component_relation_t* cr = &er->comps[comp_rel_idx++];
<<<<<<< HEAD
        cr->comp = GameCalloc("ParseRelations", MAX_NAME_LEN, sizeof(char));

        Json_GetString(comp_rel, "name", cr->comp);
        cJSON* ccount = cJSON_GetObjectItem(comp_rel, "count");
        cJSON* sublist = cJSON_GetObjectItem(comp_rel, "sublist");

=======

        cJSON* cname  = cJSON_GetObjectItem(comp_rel, "name");
        cJSON* ccount = cJSON_GetObjectItem(comp_rel, "count");
        cJSON* sublist = cJSON_GetObjectItem(comp_rel, "sublist");

        if (cJSON_IsString(cname))
          cr->comp = strdup(cname->valuestring);

>>>>>>> 9a78f7bda89d2d6c55d3d73a5974c422944a4594
        if (cJSON_IsNumber(ccount))
          cr->count = ccount->valueint;

        if (cJSON_IsArray(sublist))
        {
          int pair_idx = 0;
          cJSON* pair;
          cJSON_ArrayForEach(pair, sublist)
          {
            if (pair_idx >= MAX_RELATIONS_PER_ENTITY) break;

<<<<<<< HEAD
            cJSON* ptype = cJSON_GetObjectItem(pair, "type");
            cr->pairs[pair_idx].name = GameCalloc("ParseRelations", MAX_NAME_LEN, sizeof(char));

            Json_GetString(pair, "name", cr->pairs[pair_idx].name);
=======
            cJSON* pname = cJSON_GetObjectItem(pair, "name");
            cJSON* ptype = cJSON_GetObjectItem(pair, "type");

            if (cJSON_IsString(pname))
              cr->pairs[pair_idx].name = strdup(pname->valuestring);
>>>>>>> 9a78f7bda89d2d6c55d3d73a5974c422944a4594

            if (cJSON_IsString(ptype))
              cr->pairs[pair_idx].type = RelationTypeLookup(ptype->valuestring);
            pair_idx++;
          }
        }
      }
    }
<<<<<<< HEAD

    return true;
  }

  return false;
}

bool ParseGameDefinition(cJSON* root, game_t* out){
  if (!root || !out) return false;
  memset(out, 0, sizeof(*out));

  // ===================== SYSTEMS  ======================
  if(!ParseSystems(root, out)){
    TraceLog(LOG_ERROR, "===PARSE GAME DEF===\n Systems parse error");
    return false;
  }

  // ==================== COMPONENTS======================

  if(!ParseComponents(root, out)){
    TraceLog(LOG_ERROR, "===PARSE GAME DEF===\n Components parse error");
    return false;
  }

  // ====================== PREFABS ======================
  if(!ParsePrefabs(root, out)){
    TraceLog(LOG_ERROR, "===PARSE GAME DEF===\n Prefabs parse error");
    return false;
  }

  // ====================== RELATIONS ======================
  if(!ParseRelations(root, out)){
    TraceLog(LOG_ERROR, "===PARSE GAME DEF===\n Relations parse error");
    return false;
=======
>>>>>>> 9a78f7bda89d2d6c55d3d73a5974c422944a4594
  }

  return true;

}
