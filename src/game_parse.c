#include "game_assets.h"
#include "game_strings.h"

SheetID StringToSheetID(const char* str)
{
    if (strcmp(str, "SHEET_CHAR") == 0) return SHEET_CHAR;
    if (strcmp(str, "SHEET_MOB") == 0)  return SHEET_MOB;
    if (strcmp(str, "SHEET_TILE") == 0) return SHEET_TILE;
    // ... add others
    return SHEET_ALL;
}

// Parse the JSON file
sprite_sheet_d LoadSpriteSheet(SheetID sid, const char* jsonPath, const char* imagePath)
{
/*
  sprite_sheet_d sheet = {0};

  // Load JSON
  char* jsonData = LoadFileText(jsonPath);
  if (!jsonData) {
    TraceLog(LOG_WARNING,"Failed to load %s\n", jsonPath);
    return sheet;
  }

  cJSON* root = cJSON_Parse(jsonData);
  UnloadFileText(jsonData);

  if (!root) {
    TraceLog(LOG_WARNING,"JSON parse error\n");
    return sheet;
  }

  cJSON* framesObj = cJSON_GetObjectItem(root, "frames");
  if (!framesObj || !cJSON_IsObject(framesObj)) {
    TraceLog(LOG_WARNING,"No 'frames' object found\n");
    cJSON_Delete(root);
    return sheet;
  }

  // Count frames
  sheet.num_sprites = cJSON_GetArraySize(framesObj);

  int index = 0;
  cJSON* frameItem = NULL;
  cJSON_ArrayForEach(frameItem, framesObj)
  {
    cJSON* frameData = cJSON_GetObjectItem(frameItem, "frame");
    cJSON* duration  = cJSON_GetObjectItem(frameItem, "duration");

    if (frameData) {
      sheet.sprites[index].slice.sheet = sid;
      sheet.sprites[index].slice.scale = 1;
      sheet.sprites[index].slice.bounds.x = (float)cJSON_GetObjectItem(frameData, "x")->valueint;
      sheet.sprites[index].slice.bounds.y = (float)cJSON_GetObjectItem(frameData, "y")->valueint;
      sheet.sprites[index].slice.bounds.width  = (float)cJSON_GetObjectItem(frameData, "w")->valueint;
      sheet.sprites[index].slice.bounds.height = (float)cJSON_GetObjectItem(frameData, "h")->valueint;
    }

    if (duration) {
      sheet.sprites[index].duration = cJSON_GetObjectItem(frameItem, "duration")->valueint;
    } else {
      sheet.sprites[index].duration = 100; // default
    }

    index++;
  }

  // Load the texture
  sheet.texture = LoadTexture(imagePath);

  cJSON_Delete(root);
  return sheet;
*/
}

cJSON* ParseRoot(const char* path)
{
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

bool LoadSceneAnimData(const char* path, const char* name, anim_d* out)
{
  cJSON* root = ParseRoot(path);

  cJSON* animations = cJSON_GetObjectItem(root, "animations");
  if (!animations) {
    TraceLog(LOG_ERROR, "No 'animations' object in JSON");
    cJSON_Delete(root);
    //UnloadFileText(json_str);
    return false;
  }

  cJSON* entity_anim = cJSON_GetObjectItem(animations, name);
  if (!entity_anim) {
    TraceLog(LOG_WARNING, "No animation data for entity: %s", name);
    cJSON_Delete(root);
    //UnloadFileText(json_str);
    return false;
  }

  cJSON* sheet_item = cJSON_GetObjectItem(entity_anim, "sheet");
  if (sheet_item)
    out->sheet = StringToSheetID(sheet_item->valuestring);

  // Get sequences object
  cJSON* sequences = cJSON_GetObjectItem(entity_anim, "sequences");
  if (!sequences) {
    TraceLog(LOG_ERROR, "'sequences' object not found for %s", name);
    cJSON_Delete(root);

    return false;
  }
  SheetID sheet = StringToSheetID(cJSON_GetObjectItem(entity_anim, "sheet")->valuestring);

  out->sheet = sheet;
  const char* seq_names[] = {"idle", "walk" /* add more later */};
  int seq_ids[] = {ANIM_IDLE, ANIM_WALK};

  for (int s = 0; s < 2; s++) {                    // for each sequence type
    cJSON* seq = cJSON_GetObjectItem(sequences, seq_names[s]);
    if (!seq) continue;

    bool loop = cJSON_GetObjectItem(seq, "loop")->valueint;
    cJSON* groups = cJSON_GetObjectItem(seq, "groups");

    for (int i = 0; i < MAX_DIRECTIONS && i < cJSON_GetArraySize(groups); i++) {
      const char* group_name = cJSON_GetArrayItem(groups, i)->valuestring;

      strcpy(out->sequences[seq_ids[s]][i].name, group_name);
      out->sequences[seq_ids[s]][i].loop = loop;

      //out->num_groups[seq_ids[s]]++;
      // Handle on_end
      cJSON* on_end = cJSON_GetObjectItem(seq, "on_end");
      if (on_end && on_end->valuestring) {
        if (strcmp(on_end->valuestring, "AnimIdle") == 0)
          out->sequences[seq_ids[s]][i].on_end = AnimIdle;
        // add more callbacks as needed
      }
    }
  }

  cJSON_Delete(root);

  return true;
}

Texture2D* LoadAsepriteSheet(const char* json_path, ase_sprite_sheet_d* sheet)
{
  if (!sheet) return NULL;
  memset(sheet, 0, sizeof(ase_sprite_sheet_d));

  Texture2D* out = GameCalloc("LoadAsepriteSheet", 1, sizeof(Texture2D));
  char* text = LoadFileText(json_path);
  if (!text) {
    TraceLog(LOG_ERROR, "Failed to read file: %s", json_path);
    return NULL;
  }

  cJSON* root = cJSON_Parse(text);
  if (!root) {
    TraceLog(LOG_ERROR, "JSON parse failed: %s", json_path);
    UnloadFileText(text);
    return NULL;
  }

  cJSON* meta = cJSON_GetObjectItem(root, "meta");

  // ====================== LOAD TEXTURE ======================
  cJSON* image_item = cJSON_GetObjectItem(meta, "image");
  if (image_item && image_item->valuestring) {
    *out = LoadTexture(TextFormat("resources/%s",image_item->valuestring));
    TraceLog(LOG_INFO, "Loaded texture: %s (%dx%d)", 
        image_item->valuestring, 
        out->width, out->height);
  }

  // ====================== PARSE FRAMES ======================
  cJSON* frames_json = cJSON_GetObjectItem(root, "frames");
  sheet->num_frames = cJSON_GetArraySize(frames_json);

  for (int i = 0; i < sheet->num_frames && i < MAX_SPRITE_FRAMES; i++) {
    cJSON* f = cJSON_GetArrayItem(frames_json, i);
    anim_frame_t* frame = &sheet->frames[i];

    // Parse filename → tag + group
    const char* filename = cJSON_GetObjectItem(f, "filename")->valuestring;
    char name_copy[256];
    strncpy(name_copy, filename, sizeof(name_copy)-1);
    name_copy[sizeof(name_copy)-1] = '\0';

    // Remove extension
    char* dot = strrchr(name_copy, '.');
    if (dot) *dot = '\0';

    int count;
    char** parts = split_string(name_copy, '_', &count);

    frame->tag = hash_str_32(parts[0]);

    char* group_str = str_concat(parts[1], parts[2]);

    frame->index = i;
    frame->group = hash_str_64(group_str);
    TraceLog(LOG_INFO,"==== ASE PARSE ====\n found anim %s for %s - %llu", group_str, parts[0], frame->group);

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

    frame->duration = cJSON_GetObjectItem(f, "duration")->valueint;
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
    slice->num_keys = cJSON_GetArraySize(keys_json);

    for (int k = 0; k < slice->num_keys && k < MAX_SLICES; k++) {
      cJSON* key = cJSON_GetArrayItem(keys_json, k);
      cJSON* b = cJSON_GetObjectItem(key, "bounds");

      slice->keys[k].frame = cJSON_GetObjectItem(key, "frame")->valueint;
      slice->keys[k].bounds.x      = (float)cJSON_GetObjectItem(b, "x")->valuedouble;
      slice->keys[k].bounds.y      = (float)cJSON_GetObjectItem(b, "y")->valuedouble;
      slice->keys[k].bounds.width  = (float)cJSON_GetObjectItem(b, "w")->valuedouble;
      slice->keys[k].bounds.height = (float)cJSON_GetObjectItem(b, "h")->valuedouble;
    }
  }

  cJSON_Delete(root);
  UnloadFileText(text);

  TraceLog(LOG_INFO, "Aseprite sheet loaded: %d frames, %d slices", 
      sheet->num_frames, sheet->num_slices);

  return out;
}
// Cleanup
void Unloadsprite_sheet_d(sprite_sheet_d* sheet)
{
}
