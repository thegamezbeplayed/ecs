#include "game_register.h"

comp_id_t ComponentRegister(world_t* w, size_t elem_size){
  comp_id_t id = w->next_component_id++;

  component_pool_t* pool = GameCalloc("ComponentRegister", 1, sizeof(component_pool_t));

  pool->id = id;
  pool->elem_size = elem_size;
  pool->data = GameMalloc("ComponentRegister", elem_size * MAX_ENTITIES);

  // initialize sparse to -1 (meaning “not present”)
  for (int i = 0; i < MAX_ENTITIES; i++) {
    pool->sparse[i] = -1;
  }

  w->pools[id] = pool;

  return id;
}

void* ComponentAdd(world_t* w, Entity e, comp_id_t id){
  component_pool_t* pool = w->pools[id];

  int idx = pool->size++;

  pool->entities[idx] = e.id;
  pool->sparse[e.id] = idx;

  return (char*)pool->data + (idx * pool->elem_size);
}

void* ComponentGet(world_t* w, Entity e, comp_id_t id){
  component_pool_t* pool = w->pools[id];

  int idx = pool->sparse[e.id];
  if (idx == -1) return NULL;

  return (char*)pool->data + (idx * pool->elem_size);
}
