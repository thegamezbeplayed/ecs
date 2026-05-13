#include "game_systems.h"
#include "component_define.h"
#include "components.h"

const component_define_t CORE_COMPONENTS[NUM_COMP_CORE] = {
  {"Position",  sizeof(position_t)},
  {"RigidBody", sizeof(rigid_body_t)},
  {"Animation", sizeof(anim_comp_t)},
  {"Sprite",    sizeof(sprite_t)},
  {"View",      sizeof(view_comp_t)},
  {"Input",     sizeof(input_t)},
  {"Camera",    sizeof(camera_t)},
  {"Track",     sizeof(track_comp_t)},
  {"Type",      sizeof(EntityType)},
  //{"Stat",      sizeof(stat_t)},
  {"Force",     sizeof(force_t)},
  //{"Name",      0}, //TODO
  {"State",     sizeof(state_comp_t)},
  {"Follow",    sizeof(follow_comp_t)},
  {"Level",     sizeof(level_t)},
  {"Expiry",    sizeof(lifetime_t)}
};

const component_func_t COMPFUNC_LOOKUP[NUM_COMP_CORE] = {
  {"Animation",   AnimInit},
  {"Sprite",      SpriteInit},
  {"View",        ViewInit},
  {"Anim",        AnimInit},
  {"RigidBody",   RigidBodyInit},
  {"Force",       ForceInit},
  {"Camera",      CameraInit},
  {"Input",       InputInit},
  {"Position",    PositionInit},
};

component_entry_t* GetGameComponentDefine(game_t* g, const char* comp, const char* name){
  for(int i = 0; i < g->num_defs; i++){
    component_entry_t* entry = &g->comp_defs[i];
    if(strcmp(comp, entry->comp) != 0)
      continue;

    if(strcmp(name, entry->name) == 0)
      return entry;
  }

  return NULL;
}

void GameInitPrefabs(world_t* w, game_t* g){
  PrefabRegistryInit(w);
  
  for (int i = 0; i < g->num_prefabs; i++){
    prefab_entity_t def = g->prefabs[i];
    Entity prefab = PrefabCreate(w, def.name);

    for(int j = 0; j < def.num_comp; j++){
      comp_id_t cid = ComponentGetID(def.components[j]);

      if(cid == INVALID_COMPONENT){
        TraceLog(LOG_WARNING, "=== INIT PREFABS ===\n unable to find compnent %s", def.components[j]);
        continue;
      }

      ComponentInitFn fn = ComponentFuncLookup(def.components[j]);
     
      if(!fn){
        TraceLog(LOG_WARNING, "=== INIT PREFABS ===\n unable to find %s function", def.components[j]);
       continue;
      }
      component_entry_t *data = GetGameComponentDefine(g, def.components[j], def.name);

      if(!data){
        TraceLog(LOG_WARNING, "=== INIT PREFABS ===\n unable to find %s data for %s", def.components[j], def.name);

        continue;
      }
        
      if(!fn(ComponentAdd(w, prefab, cid), data))
        TraceLog(LOG_WARNING, "=== INIT PREFABS ===\n Failed to add component %s to Entity %s", def.components[j], def.name);

    }
  }
}

void GameSpawn(world_t* w, game_t* g){
  for(int i = 0; i < w->prefabs.count; i++){
    prefab_t* prefab = &w->prefabs.prefabs[i];
    if(prefab)
      PrefabSpawn(w, prefab->name, VEC_UNSET); 
  }
}
