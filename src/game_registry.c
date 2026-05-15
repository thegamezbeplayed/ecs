#include "game_define.h"
#include "game_systems.h"
#include "game_common.h"
#include "component_define.h"

void WorldInit(world_t* w) {
  // Zero everything first
  memset(w, 0, sizeof(world_t));

  // Init entity manager
  EntityInit(&w->manager);

  // Init component system
  w->next_component_id = 0;

  for (int i = 0; i < MAX_COMPONENTS; i++) {
    w->pools[i] = NULL;
  }

  // Init systems
  w->num_sys = 0;
  w->systems = GameCalloc("WorldInit", NUM_SYS, sizeof(system_t));

  w->iter = GameCalloc("WorldInit", 1, sizeof(entity_iter_t));
}

void PrefabRegistryInit(world_t* w) {
  w->prefabs.count = 0;
}

Entity PrefabCreate(world_t* w, const char* name) {
  if (w->prefabs.count >= MAX_PREFABS) {
    TraceLog(LOG_WARNING, "ERROR: Too many prefabs!\n");
    return (Entity){0};
  }

  Entity e = EntityCreatePrefab(&w->manager);
  prefab_t* p = &w->prefabs.prefabs[w->prefabs.count++];

  strncpy(p->name, name, 63);
  p->entity = e;
  p->comp_count = 0;

  return e;
}

prefab_t* PrefabFind(world_t* w, const char* name) {
  for (int i = 0; i < w->prefabs.count; i++) {
    if (strcmp(w->prefabs.prefabs[i].name, name) == 0)
      return &w->prefabs.prefabs[i];
  }
  return NULL;
}

Entity PrefabInstantiate(world_t* w, Entity prefab, Vector2 override_pos) {
  if (!EntityValid(&w->manager, prefab)) return (Entity){0};

  Entity instance = EntityCreate(&w->manager);
 
  // Copy all components from prefab to instance
  for (int i = 0; i < w->next_component_id; i++) {
    component_pool_t* pool = w->pools[i];
    if (!pool || !HasComponent(pool, prefab)) continue;

    void* src = ComponentGet(w, prefab, pool->id);
    void* dst = ComponentAdd(w, instance, pool->id);

    if (src && dst){
      memcpy(dst, src, pool->elem_size);
    }
    else
      TraceLog(LOG_WARNING, "Unable to add Comp %i - %s to Ent: %i",
         pool->id, CORE_COMPONENTS[pool->id], instance.id);
  }

  // Apply overrides
  if(!vec_compare(override_pos, VEC_UNSET)) {
    position_t* pos = GET_COMPONENT(w, instance, position_t, POS_ID);
    rigid_body_t* rb = GET_COMPONENT(w, instance, rigid_body_t, PHYS_ID);
    
    if (pos){
      pos->vpos = override_pos;
      if(rb)
        RigidBodySetPos(rb, override_pos);
    }
  }

  return instance;
}

Entity PrefabSpawn(world_t* w, const char* name, Vector2 world_pos){
  prefab_t* p = PrefabFind(w, name);
  if (!p) {
    TraceLog(LOG_WARNING, "Prefab not found: %s\n", name);
    return (Entity){0};
  }
  Entity spawn = PrefabInstantiate(w, p->entity, world_pos);

  for(int i = 0; i < p->rel_count; i++){
    relation_t* rel = &p->relations[i];
    if(!rel)
      continue;

    prefab_t* rp = PrefabFind(w, rel->name);

    Entity r = PrefabInstantiate(w, rp->entity, VEC_UNSET);

    EntityAddRelation(w, r, rel->type, spawn);
    EntityAddRelation(w, spawn, REL_Target, r);
  }

  return spawn;
}

relation_t* EntityAddRelation(world_t* w, Entity e, RelationType type, Entity target){
  if (!EntityValid(&w->manager, target)) 
    return NULL;

  w->relations[e.id] = (relation_t){ .target = target, .type = type };
  w->has_relation[e.id] = true;

  return &w->relations[e.id];
}

void EntityRemoveRelation(world_t* w, Entity e){
  if (e.id >= MAX_ENTITIES) return;
  w->has_relation[e.id] = false;
}

relation_t* EntityGetRelation(world_t* w, Entity e){
  if (!EntityValid(&w->manager, e))
    return NULL;

  if (!w->has_relation[e.id])
    return NULL;

  // Currently only one relation per entity
  return &w->relations[e.id];
}

Entity EntityGetRelationTarget(world_t* w, Entity e, RelationType rel){
  if (!EntityValid(&w->manager, e) || !w->has_relation[e.id] || w->relations[e.id].type != rel)
    return INVALID_ENTITY;
  return w->relations[e.id].target;
}

bool EntityHasRelation(world_t* w, Entity e, RelationType rel){
    return EntityGetRelationTarget(w, e, rel).id != INVALID_ENTITY.id;
}

