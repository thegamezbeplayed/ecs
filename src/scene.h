#ifndef __SCENE__
#define __SCENE__
#include "game_define.h"
#include "game_resources.h"

typedef struct {
    const char* scene_name;
    const char* json_path;
    const char* tile_path;
} SceneHeader;
const SceneHeader* SceneGetHeader(int index);
const char* SceneGetJsonPath(int index);   
const char* SceneGetName(int index);

typedef struct Scene {
  const char*       name;
  char              display_name[MAX_NAME_LEN];
  cJSON*            metadata;
  int               pixel_width, pixel_height;
  int               grid_width, grid_height;
  int               cell_width, cell_height;

  tile_instance_t*  tiles;
  int               tile_count;
  int               num_tiles;
  tile_define_t     *tile_defs;

  entity_instance_t*   entities;
  int               entity_count;
} Scene;

bool SceneLoadHeader(int sceneIndex, Scene* out);
void SceneLoadResources(void);
bool ParseScene(cJSON* root, Scene* scene);
bool ParseTiles(cJSON* root, Scene* out);

#endif
