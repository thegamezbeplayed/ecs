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
}

component_pool_t* StartComponentPool(world_t* w, comp_id_t id){
  if (w->pools[id]) return w->pools[id];

  component_pool_t* pool = GameCalloc("StartComponentPool", 1, sizeof(component_pool_t));

  pool->id = id;
  pool->elem_size = 0; // tag
  pool->data = NULL;

  for (int i = 0; i < MAX_ENTITIES; i++) {
    pool->sparse[i] = -1;
  }

  w->pools[id] = pool;
  return pool;
}


