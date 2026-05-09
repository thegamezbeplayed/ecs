#include "game_assets.h"
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
