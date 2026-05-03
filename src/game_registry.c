#include "game_register.h"
#include "game_systems.h"

void WorldInit(world_t* w, int sys_cap) {
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
  w->systems = GameCalloc("WorldInit", sys_cap, sizeof(system_t));


  InitComponentMap(COMPONENT_CAP);
}

void PrefabRegistryInit(world_t* w) {
  w->prefabs.count = 0;
}

Entity PrefabCreate(world_t* w, const char* name) {
  if (w->prefabs.count >= MAX_PREFABS) {
    printf("ERROR: Too many prefabs!\n");
    return (Entity){0};
  }

  Entity e = EntityCreatePrefab(&w->manager);
  prefab_t* p = &w->prefabs.prefabs[w->prefabs.count++];

  strncpy(p->name, name, 63);
  p->entity = e;
  p->comp_count = 0;

  printf("Prefab created: %s (id %u)\n", name, e.id);
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

    void* src = ComponentGet(w, prefab, i);
    void* dst = ComponentAdd(w, instance, i);

    if (src && dst)
      memcpy(dst, src, pool->elem_size);
  }

  // Apply overrides
  if (override_pos.x != -9999.0f) {  // special value = no override
    pos_comp_t* pc = GET_COMPONENT(w, instance, pos_comp_t, POS_ID);
    phys_comp_t* phc = GET_COMPONENT(w, instance, phys_comp_t, PHYS_ID);
    if (pc) {
      pc->pos.vpos = override_pos;
      if(phc)
        RigidBodySetPos(&phc->rb, override_pos);
    }
  }

  return instance;
}

Entity PrefabSpawn(world_t* w, const char* name, Vector2 world_pos){
  prefab_t* p = PrefabFind(w, name);
  if (!p) {
    printf("Prefab not found: %s\n", name);
    return (Entity){0};
  }

  return PrefabInstantiate(w, p->entity, world_pos);
}
