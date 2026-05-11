#include "scene.h"
#include "app_resource.h"

typedef struct {
    // Internal registry for multiple scene headers
    int registered_count;
    // ... other internal fields
} SceneLoader;

SceneLoader* SceneLoaderGet(void);

bool SceneLoad(const char* sceneName, Scene* out);
bool SceneLoadByIndex(int sceneIndex, Scene* out);
bool SceneInit(LoadQueue* l);
void SceneMapResources(LoadQueue* l);
void SceneSetup(world_t* w, Scene* s);

int  SceneLoaderGetSceneCount(void);
const char* SceneLoaderGetSceneName(int index);

// Register additional scene data headers
void SceneLoaderRegisterScene(const char* headerName, const char* sceneName);
