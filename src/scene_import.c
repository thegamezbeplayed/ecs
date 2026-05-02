#include "scene_data.h"
#include "game_import.h"

void SceneImport(world_t* w, const char* path){
  for (int i = 0; i < NUM_PREFABS; i++){
    EntityPrefab prefab_data = scene_prefab[i];

    Entity e = PrefabCreate(w, prefab_data.name);

    for(int j = 0; j < prefab_data.num_comp; j++)
      ImportPrefabComponent(w, e, prefab_data.components[j], path, prefab_data.name);
  }

}

void ImportPrefabComponent(world_t* w, Entity e, const char* comp, const char* path, const char* name){

  comp_id_t *cid = ComponentMapGetID(comp); 
  ComponentImportFn fn = ComponentMapFn(comp);

  if(fn && cid && *cid < w->next_component_id)
    fn(ComponentAdd(w, e, *cid), path, name);
}

void SpriteImport(void*,const char*, const char*){}
void PhysicsImport(void*,const char*, const char*){}
void InputImport(void*,const char*, const char*){}
void PositionImport(void*,const char*, const char*){}
void CameraImport(void*,const char*, const char*){}
void TrackingImport(void*,const char*, const char*){}

