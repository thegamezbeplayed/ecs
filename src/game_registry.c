#include "game_register.h"
#include "game_systems.h"

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


