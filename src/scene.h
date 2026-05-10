#ifndef __SCENE__
#define __SCENE__
#include "game_register.h"
#include "game_resources.h"

#define NUM_COMPONENTS  8

typedef struct {
    const char* scene_name;
    const char* json_path;        // ← New: relative path to rich data
    int         grid_width;
    int         grid_height;
    int         tile_count;
    int         entity_count;
} SceneHeader;
const SceneHeader* SceneGetHeader(int index);
const char* SceneGetJsonPath(int index);   
const char* SceneGetName(int index);

typedef struct TileInstance {
  int tile_index;      // from tileset
  int prefab_index;    // optional
  int start_x, start_y;
  int cell_x, cell_y;
  int rotation;
  bool flip_x;
  bool flip_y;
} TileInstance;

typedef struct EntityInstance {
  char*       prefab;           // "player", "slime", etc.
  int         type;             // ENT_PLAYER, ENT_MOB, ...
  float       x, y;
} EntityInstance;

typedef struct Scene {
  const char*       name;
  char              display_name[MAX_NAME_LEN];
  cJSON*            metadata;
  int               pixel_width, pixel_height;
  int               grid_width, grid_height;
  int               cell_width, cell_height;

  TileInstance*     tiles;
  int               tile_count;

  EntityInstance*   entities;
  int               entity_count;
} Scene;

bool SceneLoadHeader(int sceneIndex, Scene* out);
void SceneLoadResources(void);
bool ParseScene(cJSON* root, Scene* scene);

#endif
