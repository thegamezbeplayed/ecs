#include "scene.h"
#include "scene_data.h"
// -----------------------------------------------------------------------------
// Scene Registry Access
// -----------------------------------------------------------------------------
const SceneHeader* SceneGetHeader(int index){
    if (index < 0 || index >= NUM_SCENES) return NULL;
    return &SCENE_HEADERS[index];
}

const char* SceneGetJsonPath(int index){
    const SceneHeader* h = SceneGetHeader(index);
    return h ? h->json_path : NULL;
}

const char* SceneGetName(int index){
    const SceneHeader* h = SceneGetHeader(index);
    return h ? h->scene_name : NULL;
}

// -----------------------------------------------------------------------------
// Per-scene data access (example for scene 0)
// -----------------------------------------------------------------------------
bool SceneLoadHeader(int sceneIndex, Scene* out){
    if (!out || sceneIndex < 0 || sceneIndex >= NUM_SCENES)
        return false;

    const SceneHeader* header = &SCENE_HEADERS[sceneIndex];

    memset(out, 0, sizeof(Scene));

    out->name = header->scene_name;
    

    cJSON* root = ParseRoot(header->json_path);
    if(!root)
      return false;

    return ParseScene(root, out);
}

