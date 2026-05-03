#include "game_register.h"

static hash_map_t COMP_MAP;
static hash_map_t COMP_IMPORT;
static comp_id_t INVALID_COMPONENT = 0;

void ComponentMap(const char* name, comp_id_t* id, ComponentImportFn fn){
  uint64_t key = hash_str_64(name);
  HashPut(&COMP_MAP, key, id);
  HashPut(&COMP_IMPORT, key, fn);
}

comp_id_t* ComponentMapGetID(const char* name){
  uint64_t key = hash_str_64(name);
  return HashGet(&COMP_MAP, key);
}

ComponentImportFn ComponentMapFn(const char* name){
  uint64_t key = hash_str_64(name);
  return HashGet(&COMP_IMPORT, key);
}

void InitComponentMap(int size){
  HashInit(&COMP_MAP, next_pow2_int(size*2));
  HashInit(&COMP_IMPORT, next_pow2_int(size*2));
}

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

  void* ptr = (char*)pool->data + (idx * pool->elem_size);

  memset(ptr, 0, pool->elem_size);

  return ptr;
}

void* ComponentGet(world_t* w, Entity e, comp_id_t id){
  component_pool_t* pool = w->pools[id];

  int idx = pool->sparse[e.id];
  if (idx == -1) return NULL;

  return (char*)pool->data + (idx * pool->elem_size);
}

bool HasComponent(component_pool_t* pool, Entity e) {
  if (!pool || !EntityValid(&world.manager, e)) return false;
  if (e.id >= MAX_ENTITIES) return false;

  int idx = pool->sparse[e.id];
  return (idx >= INVALID_COMPONENT && pool->entities[idx] == e.id);
}
