#include "scene_loader.h"
#include "scene_data.h"

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

