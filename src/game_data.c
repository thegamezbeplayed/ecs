#include "component_define.h"
#include "system_define.h"
#include "game_control.h"
#include "components.h"

const component_define_t CORE_COMPONENTS[] = {
  {"Position",  sizeof(position_t)},
  {"RigidBody", sizeof(rigid_body_t)},
  {"Animation", sizeof(anim_comp_t)},
  {"Sprite",    sizeof(sprite_t)},
  {"Render",    sizeof(render_ctx_t)},
  {"Input",     sizeof(input_t)},
  {"Camera",    sizeof(camera_t)},
  {"Track",     sizeof(tracking_t)},
  {"Type",      sizeof(EntityType)},
  {"Observer",  sizeof(component_observer_t)},
  {"Subject",   sizeof(subject_component_t)},
  {"Force",     sizeof(force_t)},
  //{"Name",      0}, //TODO
  {"State",     sizeof(state_t)},
  {"Behavior",  sizeof(behavior_t)},
  {"Follow",    sizeof(follow_t)},
  {"Level",     sizeof(level_t)},
  {"Expiry",    sizeof(lifetime_t)},
  {"Particle",        sizeof(particle_t)},
  {"ParticleEmitter", sizeof(particle_emitter_t)}
};

const component_func_t COMPFUNC_LOOKUP[] = {
  {"Animation",   AnimInit},
  {"Sprite",      SpriteInit},
  {"Render",      RenderContextInit},
  {"Anim",        AnimInit},
  {"RigidBody",   RigidBodyInit},
  {"Force",       ForceInit},
  {"Camera",      CameraInit},
  {"Input",       InputInit},
  {"Position",    PositionInit},
  {"Observer",    ObserverInit},
  {"Subject",      SubjectInit},
  {"Follow",      FollowInit},
  {"Track",       TrackingInit},
  {"State",       StateInit},
  {"Behavior",       BehaviorInit},
  {"Particle",        ParticleInit},
  {"ParticleEmitter", ParticleEmitterInit}
};

component_entry_t* GetGameComponentDefine(game_t* g, const char* comp, const char* name){
  for(int i = 0; i < MAX_COMP_DEF; i++){
    component_entry_t* entry = &g->comp_defs[i];
    if(!entry)
      DO_NOTHING();

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

  for (int i = 0; i < g->relation_count; i++){
    entity_relation_t rel = g->relations[i];

    prefab_t* tar = PrefabFind(w, rel.name);

    if(!tar){
      TraceLog(LOG_WARNING, "=== GAME INIT PREFABS RELATIONS ===\n unable to find prefab %s for Comp %s", rel.name, rel.comps[0].comp);
      continue;
    }

    for(int j = 0; j < rel.count; j++){
      component_relation_t comp = rel.comps[j];

      comp_id_t rcid = ComponentGetID(comp.comp);
      if(rcid == INVALID_COMPONENT){
        TraceLog(LOG_WARNING, "=== GAME INIT PREFAB RELATIONS ===\n unable to find compnent %s", comp.comp);
        continue;

      }

      ComponentInitFn pfn = ComponentFuncLookup(comp.comp);
      
      for(int k = 0; k < comp.count; k++){
        relation_pair_t pair = comp.pairs[k];

        component_entry_t *pdata = GetGameComponentDefine(g, comp.comp, pair.name);
        if(!pdata){
          TraceLog(LOG_WARNING, "=== GAME INIT PREFAB RELATIONS ===\n unable to find %s data for %s", comp.comp, pair.name);

          continue;
        }

        Entity r = PrefabCreate(w, pair.name);

        if(!pfn(ComponentAdd(w, r, rcid), pdata)){
          TraceLog(LOG_WARNING, "=== GAME INIT PREFAB RELATIONS ===\n Failed to add component %s to Entity %s", comp.comp, pair.name);

          continue;
        }

        for(int l = 0; l < pair.num_comp; l++){
          comp_id_t add_cid = ComponentGetID(pair.components[l]);
          ComponentAdd(w, r, add_cid);
        }
        relation_t* rt = EntityAddRelation(w, r, pair.type, tar->entity);
        if(!rt){
          TraceLog(LOG_WARNING, "=== PREFAB RELATION ===\n failed to add %s relation to %i", pair.name, r.id); 
          continue;
        }

        strcpy(rt->name, pair.name);
        tar->relations[tar->rel_count++] = *rt;
      }
    }
  }
}

bool InitGameDefine(world_t* w){
  game_t* g = GameCalloc("InitGameDefine", 1, sizeof(game_t));

  ComponentInit(NUM_COMP_CORE);
  if(!LoadGameDefine(g))
    return false;

  for(int i = 0; i < g->num_sys; i++)
    SystemCreate(&world, &g->systems[i]);

  GameInitPrefabs(&world, g);
  UnloadGameDefine(g);

  return true;
}

void GameSpawn(world_t* w, game_t* g){
  for(int i = 0; i < w->prefabs.count; i++){
    prefab_t* prefab = &w->prefabs.prefabs[i];
    if(prefab)
      PrefabSpawn(w, prefab->name, VEC_UNSET); 
  }
}
