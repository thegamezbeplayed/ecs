#include "game_assets.h"

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

// Cleanup
void Unloadsprite_sheet_d(sprite_sheet_d* sheet)
{
}
