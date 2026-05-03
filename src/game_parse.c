#include "game_assets.h"

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

// Cleanup
void Unloadsprite_sheet_d(sprite_sheet_d* sheet)
{
}
