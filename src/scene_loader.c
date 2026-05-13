#include "scene_loader.h"
#include "scene_data.h"
#include "game_assets.h"

bool SceneLoadByIndex(int index, Scene* out){
  if (!SceneLoadHeader(index, out))
    return false;

  const char* jsonPath = SceneGetJsonPath(index);
  if (jsonPath) {
    cJSON* meta = ParseRoot(jsonPath);
    if (meta) {
      // Apply per-level settings (background, music, etc.)
      out->metadata = meta;
    }
  }

  return true;
}

void SceneLoadResources(void){
  for(int i = 0; i < NUM_RES; i++)
    ResourceLoad(RESOURCES[i]);

}
bool SceneInit(LoadQueue* l){
  ResourceInit(NUM_RES);

  for(int i = 0; i < NUM_RES; i++){
    strcpy(l->jobs[l->count++].path, RESOURCES[i].name);

    l->jobs[i].dest = &SHEETS[RESOURCES[i].sheet].texture;
  }

  return l->count > 0;
}

void SceneSetup(world_t* w, Scene* s){
  for(int i = 0; i < s->entity_count; i++){
    entity_instance_t e = s->entities[i];

    Vector2 pos = VEC_NEW(e.x, e.y);
    Entity spawn = PrefabSpawn(w, e.prefab, pos);

  }

  for(int i = 0; i < s->tile_count; i++){
    tile_instance_t t = s->tiles[i];
    Entity tile = PrefabSpawn(w, t.name, VEC_UNSET);
    Vector2 pos = VEC_NEW(t.cell_x * s->cell_width, t.cell_y * s->cell_height);

    position_t* p = GET_COMPONENT(w, tile, position_t, POS_ID);

    PositionSet(p, pos);
  }

}
