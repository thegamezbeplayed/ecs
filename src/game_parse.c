#include "game_assets.h"
#include "game_define.h"
#include "game_strings.h"
#include "scene.h"

static char* Json_GetString(cJSON* obj, const char* key, char* out, int max_len)
{
  cJSON* item = cJSON_GetObjectItem(obj, key);
  if (item && cJSON_IsString(item) && out) {
    strncpy(out, item->valuestring, max_len - 1);
    out[max_len-1] = '\0';
    return out;
  }
  out[0] = '\0';
  return out;
}

static int Json_GetInt(cJSON* obj, const char* key, int default_val)
{
  cJSON* item = cJSON_GetObjectItem(obj, key);
  return cJSON_IsNumber(item) ? item->valueint : default_val;
}

static float Json_GetFloat(cJSON* obj, const char* key, float default_val)
{
  cJSON* item = cJSON_GetObjectItem(obj, key);
  return cJSON_IsNumber(item) ? (float)item->valuedouble : default_val;
}

RelationType RelationTypeLookup(char* str){
  for(int i = 0; i < NUM_REL; i++){
    if (strcmp(str, RELATION_LOOKUP[i].name) == 0)
      return RELATION_LOOKUP[i].type;
  }
}

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
  Json_GetString(root, "display_name", scene->display_name, 128);

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
    scene->tiles = GameCalloc("ParseScene", scene->tile_count, sizeof(TileInstance));

    for (int i = 0; i < scene->tile_count; i++) {
      cJSON* item = cJSON_GetArrayItem(tile_array, i);
      TileInstance* t = &scene->tiles[i];
      t->tile_index = Json_GetInt(item, "tile_index", 0);
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
    scene->entities = GameCalloc("ParseScene", scene->entity_count, sizeof(EntityInstance));

    for (int i = 0; i < scene->entity_count; i++) {
      cJSON* item = cJSON_GetArrayItem(ent_array, i);
      EntityInstance* e = &scene->entities[i];
      Json_GetString(item, "prefab", e->prefab, 64);
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

      // The key is the entity name (e.g. "player")
      cJSON* rname  = cJSON_GetObjectItem(rel_item, "name");
      er->name = strdup(rname->valuestring);


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

        cJSON* cname  = cJSON_GetObjectItem(comp_rel, "name");
        cJSON* ccount = cJSON_GetObjectItem(comp_rel, "count");
        cJSON* sublist = cJSON_GetObjectItem(comp_rel, "sublist");

        if (cJSON_IsString(cname))
          cr->comp = strdup(cname->valuestring);

        if (cJSON_IsNumber(ccount))
          cr->count = ccount->valueint;

        if (cJSON_IsArray(sublist))
        {
          int pair_idx = 0;
          cJSON* pair;
          cJSON_ArrayForEach(pair, sublist)
          {
            if (pair_idx >= MAX_RELATIONS_PER_ENTITY) break;

            cJSON* pname = cJSON_GetObjectItem(pair, "name");
            cJSON* ptype = cJSON_GetObjectItem(pair, "type");

            if (cJSON_IsString(pname))
              cr->pairs[pair_idx].name = strdup(pname->valuestring);

            if (cJSON_IsString(ptype))
              cr->pairs[pair_idx].type = RelationTypeLookup(ptype->valuestring);
            pair_idx++;
          }
        }
      }
    }
  }

  return true;

}
